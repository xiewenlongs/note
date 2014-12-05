#!/bin/bash

YESTERDAY=`date --date='1 days ago' +%Y%m%d`
NGINX_LOG_PATH="/home/log/nginx/"
TMP_PATH="/home/log_tmp/"
LOG_FILENAME="umengplus-log-access.log-""$YESTERDAY"
INPUT_LOG_FILE="$NGINX_LOG_PATH""$LOG_FILENAME"
OUTPUT_DIR="/mnt/hdfs/user/sns2/"
BACKUP_DIR="/mnt/hdfs/user/sns3/"
DECRYPT_SCRIPTS_PATH="$HOME""/code/social/latest/umengplus"
HOSTNAME=`hostname`
SERVER_NODE1="trackdb"
SERVER_NODE2="cist-7.umops.us"
RETRY_TIMES=3


export PYENV_ROOT="$HOME/.pyenv"
export PATH="$PYENV_ROOT/bin:$PATH"
eval "$(pyenv init -)"

function retry_scp()
{
    # @1: from file path (abs path)
    # @2: to file path (abs path, if this is dir, it must end with '/')
    # @3: to host (e.g: sns@10.18.10.96)

    local try_max=$RETRY_TIMES
    local try=0
    local from_path=$1
    local to_path=$2
    local to_host=$3

    if [ ${to_path:(-1):1} == "/" ]; then
        file_name=`basename $from_path`
        to_path=$to_path"$file_name"
    fi

    if [ -z $to_host ]; then
        cmd="cp $from_path $to_path"
    else
        cmd="scp -P 18021 $from_path $to_host:$to_path"
    fi

    local_md5=`md5sum -t $from_path |cut -d ' ' -f 1`

    until [[ $try -ge $try_max ]]
    do
        echo "[DEBUG] : $cmd"
        echo "scp time : " $try

        $cmd

        # 通过NFS传输的文件， 虽然cp命令结束了，但是文件不一定传完了。cp刚结束就执行 md5sum <remote file>, 很可能报IO read error
        # 所以需要等一会再进行md5计算
        sleep 120
        if [ -z $to_host ]; then
            remote_md5=`md5sum $to_path |cut -d ' ' -f 1`
        else
            remote_md5=`ssh $to_host md5sum $to_path |cut -d ' ' -f 1`
        fi
        echo "[DEBUG] : remote md5= " $remote_md5
        echo "[DEBUG] : local md5= " $local_md5

        if [ "$local_md5"x == "$remote_md5"x ]; then
            break
        fi

        ((try++))
        sleep 1;
    done
}

cd $TMP_PATH
rm *.log

# 一楠需要log切分为200W行一个的文件

if [ $HOSTNAME = $SERVER_NODE1 ]; then
    split -l 2000000 $INPUT_LOG_FILE "$TMP_PATH""$LOG_FILENAME""-split-node1-"
elif [ $HOSTNAME = $SERVER_NODE2 ]; then
    split -l 2000000 $INPUT_LOG_FILE "$TMP_PATH""$LOG_FILENAME""-split-node2-"
fi

cd $DECRYPT_SCRIPTS_PATH

python offlineworks/tools/decrypt_nginx_log.py $TMP_PATH

#保证解密彻底完成，压缩时不会再写入, 否则下一步压缩的时候，同时写会压缩失败
sleep 10


# node 1 use NFS dist, so we just need move log into it
if [ $HOSTNAME = $SERVER_NODE1 ]; then
    if [ ! -d "$OUTPUT_DIR""$YESTERDAY" ]; then
        mkdir -p "$OUTPUT_DIR""$YESTERDAY"
    fi
    if [ ! -d "$BACKUP_DIR""$YESTERDAY" ]; then
        mkdir -p "$BACKUP_DIR""$YESTERDAY"
    fi

    #先备份全部, 再压缩(因为这里可能存在bug, 文件还没解密完成，就开始压缩，会
    #压缩失败，所以先备份, 保证上一步解密彻底完成，压缩时不会再写入

    for decrypt_filename in `ls "$TMP_PATH""$LOG_FILENAME"*"-decrypt.log"`
    do
        rsync -avzc  --progress $decrypt_filename "$BACKUP_DIR""$YESTERDAY"
    done


    for decrypt_filename in `ls "$TMP_PATH""$LOG_FILENAME"*"-decrypt.log"`
    do
        lzop -U $decrypt_filename
    done

    chmod 777 "$OUTPUT_DIR""$YESTERDAY"
    for lzo_filename in `ls "$TMP_PATH""$LOG_FILENAME"*".lzo"`
    do
        retry_scp $lzo_filename "$OUTPUT_DIR""$YESTERDAY""/"
        rm $lzo_filename
    done
fi

if [ $HOSTNAME = $SERVER_NODE2 ]; then

    mkdir -p "$TMP_PATH""$YESTERDAY"
    chmod 777 "$TMP_PATH""$YESTERDAY"

    # 先备份
    rsync -avzc  --progress "$TMP_PATH""$YESTERDAY" sns@10.18.10.96:"$BACKUP_DIR"

    for decrypt_filename in `ls "$TMP_PATH""$LOG_FILENAME"*"-decrypt.log"`
    do
        rsync -avzc  --progress "$decrypt_filename" sns@10.18.10.96:"$BACKUP_DIR""/""$YESTERDAY"
    done


    # 压缩传输
    for decrypt_filename in `ls "$TMP_PATH""$LOG_FILENAME"*"-decrypt.log"`
    do
        lzop -U $decrypt_filename
    done

    rsync -avzc  --progress "$TMP_PATH""$YESTERDAY" sns@10.18.10.96:"$OUTPUT_DIR"
    for decrypt_filename in `ls "$TMP_PATH""$LOG_FILENAME"*".lzo"`
    do
        retry_scp "$decrypt_filename" "$OUTPUT_DIR""/""$YESTERDAY""/" "sns@10.18.10.96"
    done

    rm -r "$TMP_PATH"/*.lzo
fi
