===============================================
solr
===============================================


安装
---------------------------------------

solr 5 之前，可能需要打成solr.war包，放在tomcat里启动。solr 5以后，就不支持tomcat方式启动了


requirements
~~~~~~~~~~~~~~~~~~~~~~~


.. cssclass:: table-bordered
.. table::

    ===============     ===============================================
    需求                版本
    ===============     ===============================================
    java                1.7
    ===============     ===============================================


.. warning::

    solr 从5开始，不支持打成war包在web container里(如tomcat)运行了,  因为它自己就是一个独立的server



介绍
---------------------------------------


solr 家目录
~~~~~~~~~~~~~~~~~~~~~~~

下载solr并解压后，那个目录是solr的安装目录。解压完就相当于安装完了，之后，需要定一个 ``solr 家目录``, 用来放置多个core文件的目录

solr 启动的时候，必须指定家目录。 家目录有两个作用, 一是指定配置文件，二是放置index索引文件. 家目录结构一般如下::

    <solr-home-directory>/
    |
    |___solr.xml                    solr基础功能配置
    |___zoo.cfg                     如果启动cloud模式，则家目录必须有这个配置(zookeeper的配置)
    |___核心1/
        |____core.properties
        |
        |____conf/
        |    |__solrconfig.xml      solr高级功能配置
        |    |__stopwords.txt
        |    |__protwords.txt
        |    |__synonyms.txt
        |    |__managed-schema      功能和schema.xml类似
        |    |__elevate.xml
        |    |__currency.xml
        |    |__schema.xml          schema
        |
        |____data/
             |___index/
             |___tlog/


.. warning::
    schema.xml 和 managed-schema 的功能一样，都是管理solr schema 的配置文件。具体solr使用哪个，要由solrconfig.xml里的
    <schemaFactory class="ManagedIndexSchemaFactory"> 标签来控制. managed-schema是允许solr动态来修改schema


solr 有两种模式::

    SolrCloud       collections
    standalone      cores


|

使用
---------------------------------------

下载solr
~~~~~~~~~~~~~~~~~~~~~~~

在终端执行::

    wget http://archive.apache.org/dist/lucene/solr/5.0.0/solr-5.0.0.tgz -c


部署solr
~~~~~~~~~~~~~~~~~~~~~~~

为自己的项目增加solr, 分四步:

* 定义schema. 告诉solr增加的文档怎么增加到索引中

* 安装并启动solr, 创建core

* 填充要索引的文档。 一般是把database import 到solr里

* 向用户开放搜索接口


定义schema
~~~~~~~~~~~~~~~~~~~~~~~
SOLR加载数据，创建索引和数据时，核心数据结构的配置文件是schema.xml，该配置文件主要用于配置数据源，字段类型定义，
搜索类型定义等。schema.xml的配置直接影响搜索结果的准确性与效率。

.. warning::
    默认<home>/<core>/conf/solrconfig.xml 里 schemaFactory=ManagedIndexSchemaFactory, 这样的话solr使用接口管理schema,
    所以conf/schema.xml并不起作用。 修改schemaFactory=ClassicIndexSchemaFactory, 然后再创造schema.xml, 并重启solr


|

要定义field, 先要定义fieldType, fieldType 告诉solr每种类型对应的底层Java类, 例如::

    <fieldType name="text_general" class="solr.TextField" positionIncrementGap="100">
      <analyzer type="index">
        <tokenizer class="solr.StandardTokenizerFactory"/>
          <filter class="solr.StopFilterFactory" ignoreCase="true" words="stopwords.txt" />
            <!-- in this example, we will only use synonyms at query time
            <filter class="solr.SynonymFilterFactory" synonyms="index_synonyms.txt" ignoreCase="true" expand="false"/>
            -->
          <filter class="solr.LowerCaseFilterFactory"/>
      </analyzer>
      <analyzer type="query">
        <tokenizer class="solr.StandardTokenizerFactory"/>
        <filter class="solr.StopFilterFactory" ignoreCase="true" words="stopwords.txt" />
        <filter class="solr.SynonymFilterFactory" synonyms="synonyms.txt" ignoreCase="true" expand="true"/>
        <filter class="solr.LowerCaseFilterFactory"/>
      </analyzer>
    </fieldType>


定义fieldType时，可用的属性:

.. cssclass:: table-bordered
.. table::

    =========================       ==========================================================================================
    属性名                          作用
    =========================       ==========================================================================================
    name                            The name of the fieldType.  This value gets used in field definitions, in the "type"
                                    attribute.  It is strongly recommended that names consist of alphanumeric or underscore
                                    characters only and not start with a digit. This is not currently strictly enforced.
    class                           The class name that gets used to store and index the data for this type.  Note
                                    that you may prefix included class names with "solr." and Solr will automatically figure
                                    out which packages to search for the class - so "solr.TextField" will work.  If you
                                    are using a third-party class, you will probably need to have a fully qualified class
                                    name.  The fully qualified equivalent for "solr.TextField" is "org.apache.solr.schema.
                                    TextField".
    positionIncrementGap            For multivalued fields, specifies a distance between multiple values, which prevents
                                    spurious phrase matches
    autoGeneratePhraseQueries       For text fields. If true, Solr automatically generates phrase queries for adjacent terms.
                                    If false, terms must be enclosed in double-quotes to be treated as phrases.
    docValuesFormat                 Defines a custom DocValuesFormat to use for fields of this type. This requires that a
                                    schema-aware codec, such as the SchemaCodecFactory has been configured in solrconfig.xml.
    postingsFormat                  Defines a custom PostingsFormat to use for fields of this type. This requires that a
                                    schema-aware codec, such as the SchemaCodecFactory has been configured in solrconfig.xml.
    indexed                         If true, the value of the field can be used in queries to retrieve matching documents
    stored                          If true, the actual value of the field can be retrieved by queries
    docValues                       If true, the value of the field will be put in a column-oriented DocValues structure
    sortMissingFirst
    sortMissingLast                 Control the placement of documents when a sort field is not present. As of Solr 3.5,
                                    these work for all numeric fields, including Trie and date fields.
    multiValued                     If true, indicates that a single document might contain multiple values for this type
    omitNorms                       If true, omits the norms associated with this field (this disables length normalization
                                    and index-time boosting for the field, and saves some memory). Defaults to true for all
                                    primitive (non-analyzed) field types, such as int, float, data, bool, and string. Only
                                    full-text fields or fields that need an index-time boost need norms.
    omitTermFreqAndPositions        If true, omits term frequency, positions, and payloads from postings for this field.
                                    This can be a performance boost for fields that don't require that information. It also
                                    reduces the storage space required for the index. Queries that rely on position that
                                    are issued on a field with this option will silently fail to find documents. This
                                    property defaults to true for all fields that are not text fields.
    omitPositions                   Similar to omitTermFreqAndPositions but preserves term frequency information
    termVectors
    termPositions
    termOffsets                     These options instruct Solr to maintain full term vectors for each document, optionally
                                    including the position and offset information for each term occurrence in those vectors.
                                    These can be used to accelerate highlighting and other ancillary functionality,
                                    but impose a substantial cost in terms of index size. They are not necessary for typical
                                    uses of Solr
    required                        Instructs Solr to reject any attempts to add a document which does not have a value for
                                    this field. This property defaults to false.
    =========================       ==========================================================================================

.. tip::
    各种属性的使用场景，可以参考: https://cwiki.apache.org/confluence/display/solr/Field+Properties+by+Use+Case


定义field 可用的属性:

.. cssclass:: table-bordered
.. table::

    ===============     ==========================================================================================
    属性名              作用
    ===============     ==========================================================================================
    name                mandatory - the name for the field
    type                mandatory - the name of a field type from the
                        <types> fieldType section
    indexed             true if this field should be indexed (searchable or sortable)
    stored              true if this field should be retrievable
    docValues           true if this field should have doc values. Doc values are
                        useful for faceting, grouping, sorting and function queries. Although not
                        required, doc values will make the index faster to load, more
                        NRT-friendly and more memory-efficient. They however come with some
    limitations         they are currently only supported by StrField, UUIDField
                        and all Trie*Fields, and depending on the field type, they might
                        require the field to be single-valued, be required or have a default
                        value (check the documentation of the field type you're interested in
                        for more information)
    multiValued         true if this field may contain multiple values per document
    omitNorms:          (expert) set to true to omit the norms associated with
                        this field (this disables length normalization and index-time
                        boosting for the field, and saves some memory).  Only full-text
                        fields or fields that need an index-time boost need norms.
                        Norms are omitted for primitive (non-analyzed) types by default.
    termVectors         [false] set to true to store the term vector for a given field.
                        When using MoreLikeThis, fields used for similarity should be
                        stored for best performance.
    termPositions       Store position information with the term vector. This will increase storage costs.
    termOffsets         Store offset information with the term vector. This will increase storage costs.
    required            The field is required.  It will throw an error if the value does not exist
    default             a value that should be used if no value is specified when adding a document.
    ===============     ==========================================================================================


solr常用命令
~~~~~~~~~~~~~~~~~~~~~~~


启动停止服务::

    # 创建一个home dir, 里面必须有solr.xml, 如果想使用cloud模式，还必须有zoo.cfg
    ./bin/solr start -s <home dir>

    ./bin/solr status

    ./bin/solr stop -p <port>

    # 健康监测
    ./bin/solr healthcheck -c <core>


创建core::

    ./bin/solr create <core name>


查询接口::

    curl http://localhost:8983/solr/gettingstarted/select?q=video
    curl http://localhost:8983/solr/core/query -d 'q=*:*'


更新文档::

    # 删除所有文档
    curl http://localhost:8080/solr/update --data-binary "<delete><query>*:*</query></delete>" -H 'Content-type:text/xml; charset=utf-8'  
    curl http://localhost:8080/solr/update --data-binary "<commit/>" -H 'Content-type:text/xml; charset=utf-8'

扩展
---------------------------------------

solr 可以横向扩展，有两种方式:

* cloud

  类似于数据库的sharding 模式，把数据分布在多台server上。当一个查询来到时，先去各个机器查询结果，最后把每个机器的结果merge在一起

* replica

  类似于数据库的replica模式， 一个solr节点可以sync多份，那么对每一个slave结点，都可以查询。（可能slave结点会有延迟更新的问题)


优化solr的主要方式，是优化schema:

- set stored="false" for all fields possible (esp large fields) when you
  only need to search on the field but don't need to return the original
  value.

- set indexed="false" if you don't need to search on the field, but only
  return the field as a result of searching on other indexed fields.

- remove all unneeded copyField statements

- for best index size and searching performance, set "index" to false
  for all general text fields, use copyField to copy them to the
  catchall "text" field, and use that for searching.

- For maximum indexing performance, use the ConcurrentUpdateSolrServer
  java client.

- Remember to run the JVM in server mode, and use a higher logging level
  that avoids logging every request
