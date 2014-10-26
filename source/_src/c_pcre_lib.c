#include <stdio.h>
#include <string.h>
#include <regex.h>

#define SUBSLEN 10              /* 匹配子串的数量 */
#define EBUFLEN 128             /* 错误消息buffer长度 */
#define BUFLEN 1024             /* 匹配到的字符串buffer长度 */

int main()
{
    size_t          len;
    regex_t         re;             /* 存储编译好的正则表达式，正则表达式在使用之前要经过编译 */
    regmatch_t      subs[SUBSLEN];     /* 存储匹配到的字符串位置 */
    char            matched[BUFLEN];     /* 存储匹配到的字符串 */
    char            errbuf[EBUFLEN];    /* 存储错误消息 */
    int             err, i;

    char            src[] = "111 <title>Hello World</title> 222";    /* 源字符串 */
    char            pattern[] = "<title>(.*)</title>";    /* pattern字符串 */

    printf("String : %s\n", src);
    printf("Pattern: \"%s\"\n", pattern);

    /* 编译正则表达式 */
    err = regcomp(&re, pattern, REG_EXTENDED);

    if (err) {
        len = regerror(err, &re, errbuf, sizeof(errbuf));
        printf("error: regcomp: %s\n", errbuf);
        return 1;
    }
    printf("Total has subexpression: %d\n", re.re_nsub);
    /* 执行模式匹配 */
    err = regexec(&re, src, (size_t) SUBSLEN, subs, 0);

    if (err == REG_NOMATCH) { /* 没有匹配成功 */
        printf("Sorry, no match .../n");
        regfree(&re);
        return 0;
    } else if (err) {  /* 其它错误 */
        len = regerror(err, &re, errbuf, sizeof(errbuf));
        printf("error: regexec: %s/n", errbuf);
        return 1;
    }

    /* 如果不是REG_NOMATCH并且没有其它错误，则模式匹配上 */
    printf("\nOK, has matched ...\n\n");
    for (i = 0; i <= re.re_nsub; i++) {
        len = subs[i].rm_eo - subs[i].rm_so;
        if (i == 0) {
            printf ("begin: %d, len = %d  ", subs[i].rm_so, len); /* 注释1 */
        } else {
            printf("subexpression %d begin: %d, len = %d  ", i, subs[i].rm_so, len); 
        }
        memcpy (matched, src + subs[i].rm_so, len);
        matched[len] = '\0';
        printf("match: %s/n", matched);
    }

    regfree(&re);   /* 用完了别忘了释放 */
    return (0);
}
