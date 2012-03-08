<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>termsanddefs</name>
    <title>Terms and Definitions</title>
    <filename>a00001</filename>
  </compound>
  <compound kind="page">
    <name>tools</name>
    <title>Command-Line Tools</title>
    <filename>a00002</filename>
    <docanchor file="a00002">cli_hwloc_ps</docanchor>
    <docanchor file="a00002">cli_hwloc_assembler_remote</docanchor>
    <docanchor file="a00002">cli_hwloc_assembler</docanchor>
    <docanchor file="a00002">cli_hwloc_distances</docanchor>
    <docanchor file="a00002">cli_hwloc_bind</docanchor>
    <docanchor file="a00002">cli_hwloc_gather</docanchor>
    <docanchor file="a00002">cli_hwloc_distrib</docanchor>
    <docanchor file="a00002">cli_lstopo</docanchor>
    <docanchor file="a00002">cli_hwloc_calc</docanchor>
  </compound>
  <compound kind="page">
    <name>envvar</name>
    <title>Environment Variables</title>
    <filename>a00003</filename>
  </compound>
  <compound kind="page">
    <name>cpu_mem_bind</name>
    <title>CPU and Memory Binding Overview</title>
    <filename>a00004</filename>
  </compound>
  <compound kind="page">
    <name>iodevices</name>
    <title>I/O Devices</title>
    <filename>a00005</filename>
    <docanchor file="a00005">iodevices_examples</docanchor>
    <docanchor file="a00005">iodevices_enabling</docanchor>
    <docanchor file="a00005">iodevices_consult</docanchor>
    <docanchor file="a00005">iodevices_osdev</docanchor>
    <docanchor file="a00005">iodevices_hierarchy</docanchor>
  </compound>
  <compound kind="page">
    <name>multinode</name>
    <title>Multi-node Topologies</title>
    <filename>a00006</filename>
    <docanchor file="a00006">multinode_interface</docanchor>
    <docanchor file="a00006">multinode_tools</docanchor>
    <docanchor file="a00006">multinode_cpusets</docanchor>
    <docanchor file="a00006">multinode_example</docanchor>
  </compound>
  <compound kind="page">
    <name>xml</name>
    <title>Importing and exporting topologies from/to XML files</title>
    <filename>a00007</filename>
    <docanchor file="a00007">xml_backends</docanchor>
    <docanchor file="a00007">xml_errors</docanchor>
  </compound>
  <compound kind="page">
    <name>interoperability</name>
    <title>Interoperability With Other Software</title>
    <filename>a00008</filename>
  </compound>
  <compound kind="page">
    <name>threadsafety</name>
    <title>Thread Safety</title>
    <filename>a00009</filename>
  </compound>
  <compound kind="page">
    <name>embed</name>
    <title>Embedding hwloc in Other Software</title>
    <filename>a00010</filename>
    <docanchor file="a00010">embedding_example</docanchor>
    <docanchor file="a00010">embedding_m4</docanchor>
  </compound>
  <compound kind="page">
    <name>faq</name>
    <title>Frequently Asked Questions</title>
    <filename>a00011</filename>
    <docanchor file="a00011">faq_smt</docanchor>
    <docanchor file="a00011">faq_valgrind</docanchor>
    <docanchor file="a00011">faq_onedim</docanchor>
    <docanchor file="a00011">faq_privileged</docanchor>
    <docanchor file="a00011">faq_upgrade</docanchor>
    <docanchor file="a00011">faq_asymmetric</docanchor>
    <docanchor file="a00011">faq_xml</docanchor>
    <docanchor file="a00011">faq_annotate</docanchor>
  </compound>
  <compound kind="group">
    <name>hwlocality_api_version</name>
    <title>API version</title>
    <filename>a00038.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>HWLOC_API_VERSION</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga8f4dfb8eef138af55dd1a0fa802e5476</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC unsigned</type>
      <name>hwloc_get_api_version</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga61ef7566efe550d314b0ce4f3421ec5d</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_topology</name>
    <title>Topology context</title>
    <filename>a00039.html</filename>
    <member kind="typedef">
      <type>struct hwloc_topology *</type>
      <name>hwloc_topology_t</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga9d1e76ee15a7dee158b786c30b6a6e38</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_sets</name>
    <title>Object sets (hwloc_cpuset_t and hwloc_nodeset_t)</title>
    <filename>a00040.html</filename>
    <member kind="typedef">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_cpuset_t</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>ga4bbf39b68b6f568fb92739e7c0ea7801</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>hwloc_const_bitmap_t</type>
      <name>hwloc_const_cpuset_t</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>ga1f784433e9b606261f62d1134f6a3b25</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_nodeset_t</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>ga37e35730fa7e775b5bb0afe893d6d508</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>hwloc_const_bitmap_t</type>
      <name>hwloc_const_nodeset_t</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>ga2f5276235841ad66a79bedad16a5a10c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_types</name>
    <title>Topology Object Types</title>
    <filename>a00041.html</filename>
    <member kind="typedef">
      <type>enum hwloc_obj_bridge_type_e</type>
      <name>hwloc_obj_bridge_type_t</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0a947e8c5adcc729b126bd09c01a0153</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum hwloc_obj_osdev_type_e</type>
      <name>hwloc_obj_osdev_type_t</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga90c1e82a60ba5871d07645169e636987</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>hwloc_obj_type_t</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gacd37bb612667dc437d66bfb175a8dc55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_SYSTEM</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a3aa1b842d1fd4207ebce171f95a244ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_MACHINE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a3f4e83ffc4a259354959ae8a9eaa2a80</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_NODE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55aaf0964881117bdedf1a5e9332cd120dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_SOCKET</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a1ac6e07775ae4324b3fe9dbd72c785ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_CACHE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a56ee0b7eca88f363b75b34fdde8c9ddc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_CORE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55ac793958f330bca371aa1535de8aff45f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_PU</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55abca6887e80cb291353b0a0c1da83f661</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_GROUP</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a5269ef95be72f88465559d35c9b7ad56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_MISC</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a19f8a6953fa91efc76bcbcdf2d22de4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_BRIDGE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a6825f10895fea60aca7a6ba9fe273db0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_PCI_DEVICE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a5d8117a54df1fbd3606ab19e42cb0ea9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_OS_DEVICE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a51e7280240fd9f25589cbbe538bdb075</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_TYPE_MAX</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55addb5f843e1812445a84e6b2a844b1ebc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>hwloc_obj_bridge_type_e</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga48a4803c72574191d7ead1c62aaf9860</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_BRIDGE_HOST</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga48a4803c72574191d7ead1c62aaf9860a2c7660f3864ad2810c1e72aad285e574</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_BRIDGE_PCI</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga48a4803c72574191d7ead1c62aaf9860a8f3b4cecf3dab6073d74696d10863c60</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>hwloc_obj_osdev_type_e</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga64f5d539df299c97ae80ce53fc4b56c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_OSDEV_BLOCK</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a689b0488c3c0d08d116751c6b9cb8871</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_OSDEV_GPU</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0aa3a09798ef2836abb236dc3a645ffc90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_OSDEV_NETWORK</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0ab715d81155f771573c8682dffc65021b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_OSDEV_OPENFABRICS</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a52157d03694fdae82dddd57ca8c973b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_OBJ_OSDEV_DMA</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a827ad1643360711a8b6c6af671366791</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>hwloc_compare_types_e</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga46323568968005137c32f6a1cd405b74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TYPE_UNORDERED</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gga46323568968005137c32f6a1cd405b74a2f8297ea36eba46e7596e810a67298fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_compare_types</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gabd7da4f4ea12b420b8ecbde458b27805</anchor>
      <arglist>(hwloc_obj_type_t type1, hwloc_obj_type_t type2) __hwloc_attribute_const</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_objects</name>
    <title>Topology Objects</title>
    <filename>a00042.html</filename>
    <class kind="struct">hwloc_obj_memory_s</class>
    <class kind="struct">hwloc_obj</class>
    <class kind="union">hwloc_obj_attr_u</class>
    <class kind="struct">hwloc_distances_s</class>
    <class kind="struct">hwloc_obj_info_s</class>
    <member kind="typedef">
      <type>struct hwloc_obj *</type>
      <name>hwloc_obj_t</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga79b8ab56877ef99ac59b833203391c7d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_creation</name>
    <title>Create and Destroy Topologies</title>
    <filename>a00043.html</filename>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_init</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga5c2d6f476af87005c7bd0811d4548b9f</anchor>
      <arglist>(hwloc_topology_t *topologyp)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_load</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga91e2e6427b95fb7339c99dbbef996e71</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_topology_destroy</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga6040925d3ee4bbb2647f2a321aca5f4b</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_topology_check</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gab3628b2a540a5a08e8cf724ef829e70a</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_configuration</name>
    <title>Configure Topology Detection</title>
    <filename>a00044.html</filename>
    <class kind="struct">hwloc_topology_discovery_support</class>
    <class kind="struct">hwloc_topology_cpubind_support</class>
    <class kind="struct">hwloc_topology_membind_support</class>
    <class kind="struct">hwloc_topology_support</class>
    <member kind="enumeration">
      <name>hwloc_topology_flags_e</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gada025d3ec20b4b420f8038d23d6e7bde</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea129b4fea1300be22bbaf0bb0958994c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea6ecb6abc6a0bb75e81564f8bca85783b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TOPOLOGY_FLAG_IO_DEVICES</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea46ae25e8896278840b1800ae9ce4de41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TOPOLOGY_FLAG_IO_BRIDGES</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea426b18c349f15d7046bb391d96fa947c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TOPOLOGY_FLAG_WHOLE_IO</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea4a41dc181649ef81c2dcd44a54e271b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_ignore_type</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gaf2071c8621fddc53649c245d87835b47</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_ignore_type_keep_structure</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ga6ddd4213d95bd1c30555b294a60efa6b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_ignore_all_keep_structure</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gaec6fb00050f50cd41007f1ae580d2106</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_flags</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ga6d11e53db143ac39c32cdb3912b71f99</anchor>
      <arglist>(hwloc_topology_t topology, unsigned long flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_pid</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gae1100de0162b3c6a9db750ac14629c05</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, hwloc_pid_t pid)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_fsroot</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ga2f6bfb6958d8b508ea1d7d5bb266432c</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, const char *__hwloc_restrict fsroot_path)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_synthetic</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ga2fcb52181b586c20f001b7a999550324</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, const char *__hwloc_restrict description)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_xml</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ga93efcc8a962afe1ed23393700682173f</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, const char *__hwloc_restrict xmlpath)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_xmlbuffer</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gae7e4bade144652a2b48f5eaf0309b4ec</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, const char *__hwloc_restrict buffer, int size)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_custom</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>ga12024fec46f9368fb8fc5c624089c5ec</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_set_distance_matrix</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gabda6afa67a495cd652f064ad51d3fe47</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, hwloc_obj_type_t type, unsigned nbobjs, unsigned *os_index, float *distances)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC struct hwloc_topology_support *</type>
      <name>hwloc_topology_get_support</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>gac2126e105f3ae708efca2e90d612625a</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_tinker</name>
    <title>Tinker With Topologies.</title>
    <filename>a00045.html</filename>
    <member kind="enumeration">
      <name>hwloc_restrict_flags_e</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>ga9d80f08eb25b7ac22f1b998dc8bf521f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_RESTRICT_FLAG_ADAPT_DISTANCES</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>gga9d80f08eb25b7ac22f1b998dc8bf521fa4d18407f5520793b50b9e892f5bb55d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_RESTRICT_FLAG_ADAPT_MISC</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>gga9d80f08eb25b7ac22f1b998dc8bf521fa699969227a09bbc1a7de51dc9fb7be4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_RESTRICT_FLAG_ADAPT_IO</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>gga9d80f08eb25b7ac22f1b998dc8bf521faa95d6985e36ec1e55f68b210297a85cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_export_xml</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>ga45578d725c66865cfef31d0585dcff70</anchor>
      <arglist>(hwloc_topology_t topology, const char *xmlpath)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_export_xmlbuffer</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>ga739330e9402425315e44e5012631fb91</anchor>
      <arglist>(hwloc_topology_t topology, char **xmlbuffer, int *buflen)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_free_xmlbuffer</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>ga5e375acef034bebc1f20ead884697301</anchor>
      <arglist>(hwloc_topology_t topology, char *xmlbuffer)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_obj_t</type>
      <name>hwloc_topology_insert_misc_object_by_cpuset</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>ga017a9ba16d554326c6e3812d545d7230</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_obj_t</type>
      <name>hwloc_topology_insert_misc_object_by_parent</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>gadacd7a3d21220fbb30c3256d8b22a294</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t parent, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_restrict</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>gad75fa918e3eb54663bdeab25ed89b648</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology, hwloc_const_cpuset_t cpuset, unsigned long flags)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_information</name>
    <title>Get Some Topology Information</title>
    <filename>a00046.html</filename>
    <member kind="enumeration">
      <name>hwloc_get_type_depth_e</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>gaf4e663cf42bbe20756b849c6293ef575</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TYPE_DEPTH_UNKNOWN</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575a0565ab92ab72cb0cec91e23003294aad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TYPE_DEPTH_MULTIPLE</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575ae99465995cacde6c210d5fc2e409798c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TYPE_DEPTH_BRIDGE</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575af93b50182973e4a718d9d4db9e253a90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TYPE_DEPTH_PCI_DEVICE</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575ad8b1516e699b57ce1c8d107fbd2f674c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_TYPE_DEPTH_OS_DEVICE</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575afe9a2131073eebbe129d4aa2928d3f46</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC unsigned</type>
      <name>hwloc_topology_get_depth</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ga8c30b0cec55074eb3ed34e4f2a1a9937</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_type_depth</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>gaea7c64dd59467f5201ba87712710b14d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_obj_type_t</type>
      <name>hwloc_get_depth_type</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>gadd4964764ae7e49231065d58a553fd31</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC unsigned</type>
      <name>hwloc_get_nbobjs_by_depth</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ga20cfe2456f4cfdd789c9aca6d2fdd69f</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_get_nbobjs_by_type</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>gaba821f84ef64282d14577066e6d6547e</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_topology_is_thissystem</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>ga0d109e33fc7990f62f665d336e5e5111</anchor>
      <arglist>(hwloc_topology_t __hwloc_restrict topology) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_traversal</name>
    <title>Retrieve Objects</title>
    <filename>a00047.html</filename>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_obj_t</type>
      <name>hwloc_get_obj_by_depth</name>
      <anchorfile>a00047.html</anchorfile>
      <anchor>gaedd78240b0c1108355586a268ec5a697</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth, unsigned idx) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_obj_by_type</name>
      <anchorfile>a00047.html</anchorfile>
      <anchor>ga9be4a03488cdd0fb431e4aa1cbdea895</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type, unsigned idx) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_conversion</name>
    <title>Object/String Conversion</title>
    <filename>a00048.html</filename>
    <member kind="function">
      <type>HWLOC_DECLSPEC const char *</type>
      <name>hwloc_obj_type_string</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>ga7c61920feca6fd9006d930dabfc09058</anchor>
      <arglist>(hwloc_obj_type_t type) __hwloc_attribute_const</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_obj_type_t</type>
      <name>hwloc_obj_type_of_string</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>gade722091ae392fdc79557e797a16c370</anchor>
      <arglist>(const char *string) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_obj_type_snprintf</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>ga3ad856e8f3487d340c82a23b8a2a0351</anchor>
      <arglist>(char *__hwloc_restrict string, size_t size, hwloc_obj_t obj, int verbose)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_obj_attr_snprintf</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>ga0db8286d7f3ceda8defd76e3e1e2b284</anchor>
      <arglist>(char *__hwloc_restrict string, size_t size, hwloc_obj_t obj, const char *__hwloc_restrict separator, int verbose)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_obj_snprintf</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>ga5c6a61a83f4790b421e2f62e9088446f</anchor>
      <arglist>(char *__hwloc_restrict string, size_t size, hwloc_topology_t topology, hwloc_obj_t obj, const char *__hwloc_restrict indexprefix, int verbose)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_obj_cpuset_snprintf</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>gabbfb92224c992c0e2ecef6b6e45260f2</anchor>
      <arglist>(char *__hwloc_restrict str, size_t size, size_t nobj, const hwloc_obj_t *__hwloc_restrict objs)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline const char *</type>
      <name>hwloc_obj_get_info_by_name</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>ga2a12b2450489675bd5759dc47cbc51ed</anchor>
      <arglist>(hwloc_obj_t obj, const char *name) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_obj_add_info</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>gaba3afe636940872772ed6dfaf0b3552e</anchor>
      <arglist>(hwloc_obj_t obj, const char *name, const char *value)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_cpubinding</name>
    <title>CPU binding</title>
    <filename>a00049.html</filename>
    <member kind="enumeration">
      <name>hwloc_cpubind_flags_t</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>ga217dc8d373f8958cc93c154ebce1c71c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_CPUBIND_PROCESS</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71ca2e0dd0128dac6b03408c7dd170477fdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_CPUBIND_THREAD</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71caf1b6bbad00d7b1017b918e3719f4d421</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_CPUBIND_STRICT</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71ca679a7e0f0c7ee06b123565f90d98e7fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_CPUBIND_NOMEMBIND</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71ca41ce440443cc3087caed95ab60edcad6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_cpubind</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gaf4cc194d5c0d38004a21b9f03522a7e3</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_cpubind</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gab52f83ab16eac465da55e16bdd30df77</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_proc_cpubind</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gac0ef3510b4e9c1273029ecf94e8c9686</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_proc_cpubind</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gaa1373a002b9a0a87eb002e26c590f2da</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_thread_cpubind</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>gaab65c34a0bc19c5a823f824543cedbd4</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_thread_t thread, hwloc_const_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_thread_cpubind</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>ga089cecfdb9a50e5c5cd1b6ff17352d42</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_thread_t thread, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_last_cpu_location</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>ga8e9a4b5ee3eaa18fd3a229790c6b5b17</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_proc_last_cpu_location</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>ga1e005ddb9701e6a993a9eb8abe67a710</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t set, int flags)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_membinding</name>
    <title>Memory binding</title>
    <filename>a00050.html</filename>
    <member kind="enumeration">
      <name>hwloc_membind_policy_t</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gac9764f79505775d06407b40f5e4661e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_DEFAULT</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8a18675bb80ebc1bce5b652e9de8f3998c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_FIRSTTOUCH</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8a979c7aa78dd32780858f30f47a72cca0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_BIND</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8ad811fa4b2a6002c4d63695a408ffde2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_INTERLEAVE</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8ae370075e5af016d42310f87ea5af236b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_REPLICATE</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8afb37480fe5f4236eb7dd4aef26f691e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_NEXTTOUCH</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8aecdd4164d647708fbb51a00d98dbb138</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_MIXED</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8a3185bd869b67817fb2bd5164bf360402</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>hwloc_membind_flags_t</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gab00475fd98815bf4fb9aaf752030e7d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_PROCESS</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2a1b1b74aef138f64aff214a8cbdfe8eb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_THREAD</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2a1dc7dd5cdcd5796893a325a524555298</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_STRICT</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2a0335311a0ee04166df2888d52b4a42c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_MIGRATE</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2aa6e49e54f52827cb143cc869cfd748af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HWLOC_MEMBIND_NOCPUBIND</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2aad6b9eaf2ee324ca58dc8f58094b9997</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga747962cbb16fd12ad6d126011c734a27</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga8b6d1d90227aff8e44ef26bc1f8a8f95</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga7b46a4ca0b2db9723ca73b0c148ccd93</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gab07aedba1119f99130e5f0b796b7d8a5</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_proc_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gaf10d18cd4703ec980d7440a1c23fa07f</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_proc_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gac63c4517e71b3f0d41b3dee3e83dc55c</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_proc_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga22074ed072e1039f74f17f7b16e9e39d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_nodeset_t nodeset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_proc_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gabaf9c13f1fa301d6895594cec52226e2</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t cpuset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_area_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gade5e2c28ea8475a479bf2b1df36c6ccd</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_set_area_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga06594edeb4728cb9e014aed924d1e74a</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_area_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga106e6fcd764d17be682faf7a5bc95720</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_nodeset_t nodeset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_area_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gac02ee8f7606382ea7855ae4e1656ed0f</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_cpuset_t cpuset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void *</type>
      <name>hwloc_alloc</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gac5586e58cf25c3596b7d4aa31ce13259</anchor>
      <arglist>(hwloc_topology_t topology, size_t len)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void *</type>
      <name>hwloc_alloc_membind_nodeset</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>gaeaa00714a9c4319bda0a74ca6f8720e8</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void *</type>
      <name>hwloc_alloc_membind</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga221a7edc5d436300374fa16463f607e5</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_free</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>ga986d9b4cc76da592c4b937c6cb7d9d56</anchor>
      <arglist>(hwloc_topology_t topology, void *addr, size_t len)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_custom</name>
    <title>Building Custom Topologies</title>
    <filename>a00051.html</filename>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_custom_insert_topology</name>
      <anchorfile>a00051.html</anchorfile>
      <anchor>ga59ccf6a63cb23d63940e8b782059d9cb</anchor>
      <arglist>(hwloc_topology_t newtopology, hwloc_obj_t newparent, hwloc_topology_t oldtopology, hwloc_obj_t oldroot)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_obj_t</type>
      <name>hwloc_custom_insert_group_object_by_parent</name>
      <anchorfile>a00051.html</anchorfile>
      <anchor>gac1037fe389bfa7c2bf0e3739b7e20844</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t parent, int groupdepth)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_types</name>
    <title>Object Type Helpers</title>
    <filename>a00052.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_get_type_or_below_depth</name>
      <anchorfile>a00052.html</anchorfile>
      <anchor>ga081be77905201e9f42318e9974456b45</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_get_type_or_above_depth</name>
      <anchorfile>a00052.html</anchorfile>
      <anchor>ga62a3f401854c209605c90079700f4bc5</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_traversal_basic</name>
    <title>Basic Traversal Helpers</title>
    <filename>a00053.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_root_obj</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>gadbf58f6e187efbdb3cd9a8e30311b7d7</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_ancestor_obj_by_depth</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ga9b9d6af74eb0b78b1384aa66dd058ddc</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, unsigned depth, hwloc_obj_t obj) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_ancestor_obj_by_type</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>gac93e1a95a71add57ac841daef1b94e5e</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_type_t type, hwloc_obj_t obj) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_obj_by_depth</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>gab7c1dce3f42ece5bfa621e87cf332418</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_obj_by_type</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ga5f08ceb69375341e73563cfe2e77534e</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_pu_obj_by_os_index</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ga0bc54225b65d557c70c7cb5dfb714de7</anchor>
      <arglist>(hwloc_topology_t topology, unsigned os_index) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_child</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>gae5ef1af636849f77714e1584ba78cf9c</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_t parent, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_common_ancestor_obj</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ga52a334f17c6b5b409d5cc6bb4ab8a2ab</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_t obj1, hwloc_obj_t obj2) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_obj_is_in_subtree</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ga08b5c1c1a95ed27a219c8af864690b3e</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_t obj, hwloc_obj_t subtree_root) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_inside</name>
    <title>Finding Objects Inside a CPU set</title>
    <filename>a00054.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_first_largest_obj_inside_cpuset</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>gabcd5fa81a95fa5335950cae092277d5b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_get_largest_objs_inside_cpuset</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>gaab04c89623662e63a48ed2cd48eb601c</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_t *__hwloc_restrict objs, int max)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_obj_inside_cpuset_by_depth</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ga8af256c2572f16520f95440b884c1bd6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_obj_inside_cpuset_by_type</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ga934e7ecd68b33403e0c0be779d9ed1e6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_obj_inside_cpuset_by_depth</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ga57c8a0e227d1b16a31d19aaf755b42c0</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth, unsigned idx) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_obj_inside_cpuset_by_type</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>gaa8dcdb85224f7350b90fb0a1ca91e6d6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type, unsigned idx) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline unsigned</type>
      <name>hwloc_get_nbobjs_inside_cpuset_by_depth</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ga457604a2dedbf70ba3b480558666b56b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_get_nbobjs_inside_cpuset_by_type</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ga63481874c69ed257b1a0c03e7615ff97</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_get_obj_index_inside_cpuset</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ga7cdb6a4645e9b1ec80137e72d1736fdd</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_const_cpuset_t set, hwloc_obj_t obj) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_covering</name>
    <title>Finding a single Object covering at least CPU set</title>
    <filename>a00055.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_child_covering_cpuset</name>
      <anchorfile>a00055.html</anchorfile>
      <anchor>ga69a22cc76c88115aee7b7e6fc53fca7d</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_const_cpuset_t set, hwloc_obj_t parent) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_obj_covering_cpuset</name>
      <anchorfile>a00055.html</anchorfile>
      <anchor>ga68300dd0ee9c36a7a90f275a59e9af28</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_coverings</name>
    <title>Finding a set of similar Objects covering at least a CPU set</title>
    <filename>a00056.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_obj_covering_cpuset_by_depth</name>
      <anchorfile>a00056.html</anchorfile>
      <anchor>ga2f9a4ec15e9cfae8c21501257a51ce5b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_obj_covering_cpuset_by_type</name>
      <anchorfile>a00056.html</anchorfile>
      <anchor>ga5915ea30f326676b3a4cfff371ce04d1</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type, hwloc_obj_t prev)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_cache</name>
    <title>Cache-specific Finding Helpers</title>
    <filename>a00057.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_cache_covering_cpuset</name>
      <anchorfile>a00057.html</anchorfile>
      <anchor>ga2f936fd9a9c62f6d9b9951de3062e889</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_shared_cache_covering_obj</name>
      <anchorfile>a00057.html</anchorfile>
      <anchor>ga000e08a1da039130daa072e77713bb43</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_t obj) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_traversal</name>
    <title>Advanced Traversal Helpers</title>
    <filename>a00058.html</filename>
    <member kind="function">
      <type>HWLOC_DECLSPEC unsigned</type>
      <name>hwloc_get_closest_objs</name>
      <anchorfile>a00058.html</anchorfile>
      <anchor>ga26c2ac4f25b1ed293249c88e232f1bea</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t src, hwloc_obj_t *__hwloc_restrict objs, unsigned max)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_obj_below_by_type</name>
      <anchorfile>a00058.html</anchorfile>
      <anchor>ga6c482c2097f01b1ff88598fec8a1ba3b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type1, unsigned idx1, hwloc_obj_type_t type2, unsigned idx2) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_obj_below_array_by_type</name>
      <anchorfile>a00058.html</anchorfile>
      <anchor>gac33a0990f4e771b2117ddedabc3f00f6</anchor>
      <arglist>(hwloc_topology_t topology, int nr, hwloc_obj_type_t *typev, unsigned *idxv) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_binding</name>
    <title>Binding Helpers</title>
    <filename>a00059.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void</type>
      <name>hwloc_distributev</name>
      <anchorfile>a00059.html</anchorfile>
      <anchor>gaf057d7c5e3cb3df897ce527258537619</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t *root, unsigned n_roots, hwloc_cpuset_t *cpuset, unsigned n, unsigned until)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void</type>
      <name>hwloc_distribute</name>
      <anchorfile>a00059.html</anchorfile>
      <anchor>ga6d5c88292ad5aa062c1bebc99369c042</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t root, hwloc_cpuset_t *cpuset, unsigned n, unsigned until)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void *</type>
      <name>hwloc_alloc_membind_policy_nodeset</name>
      <anchorfile>a00059.html</anchorfile>
      <anchor>ga3e772fbc4de626ed80f13d332b7d4d03</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void *</type>
      <name>hwloc_alloc_membind_policy</name>
      <anchorfile>a00059.html</anchorfile>
      <anchor>ga6178c6a9ec1dd88ec9f6a9fcdcc7d634</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_cpuset</name>
    <title>Cpuset Helpers</title>
    <filename>a00060.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_complete_cpuset</name>
      <anchorfile>a00060.html</anchorfile>
      <anchor>ga418ebb39eaf1eac8f9cf4047cf59a534</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_topology_cpuset</name>
      <anchorfile>a00060.html</anchorfile>
      <anchor>gaa319133e702fea664750c45735ac8a25</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_online_cpuset</name>
      <anchorfile>a00060.html</anchorfile>
      <anchor>gafeb07985e2cc66aee1da447aff0bcab8</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_allowed_cpuset</name>
      <anchorfile>a00060.html</anchorfile>
      <anchor>ga08e3b14e1fe7642b383633d34c1f3c4a</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_nodeset</name>
    <title>Nodeset Helpers</title>
    <filename>a00061.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_nodeset_t</type>
      <name>hwloc_topology_get_complete_nodeset</name>
      <anchorfile>a00061.html</anchorfile>
      <anchor>gaddb7a1f3d71510bb011189e0b8edf29f</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_nodeset_t</type>
      <name>hwloc_topology_get_topology_nodeset</name>
      <anchorfile>a00061.html</anchorfile>
      <anchor>ga78f80a3c04718ca273791a1071512031</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_const_nodeset_t</type>
      <name>hwloc_topology_get_allowed_nodeset</name>
      <anchorfile>a00061.html</anchorfile>
      <anchor>ga13915b8e074f59fbdf8f973739509293</anchor>
      <arglist>(hwloc_topology_t topology) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_nodeset_convert</name>
    <title>Conversion between cpuset and nodeset</title>
    <filename>a00062.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void</type>
      <name>hwloc_cpuset_to_nodeset</name>
      <anchorfile>a00062.html</anchorfile>
      <anchor>ga60ecc4ae480c28b5fbd34aca4fc37daa</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, hwloc_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void</type>
      <name>hwloc_cpuset_to_nodeset_strict</name>
      <anchorfile>a00062.html</anchorfile>
      <anchor>ga9162785e39d7c697f76f99524c4a2fb4</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_const_cpuset_t cpuset, hwloc_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void</type>
      <name>hwloc_cpuset_from_nodeset</name>
      <anchorfile>a00062.html</anchorfile>
      <anchor>gaa677fd588304b5615de4ea78104adfb5</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, hwloc_const_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline void</type>
      <name>hwloc_cpuset_from_nodeset_strict</name>
      <anchorfile>a00062.html</anchorfile>
      <anchor>gaa7c3f39802b00a758c58e024a8119979</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_cpuset_t cpuset, hwloc_const_nodeset_t nodeset)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_distances</name>
    <title>Distances</title>
    <filename>a00063.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline struct hwloc_distances_s *</type>
      <name>hwloc_get_whole_distance_matrix_by_depth</name>
      <anchorfile>a00063.html</anchorfile>
      <anchor>ga48dfec8b8ba1fb8f2073ecd0728f0ca6</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline struct hwloc_distances_s *</type>
      <name>hwloc_get_whole_distance_matrix_by_type</name>
      <anchorfile>a00063.html</anchorfile>
      <anchor>ga9979237728f7a00a38836e15a8e8cd17</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline struct hwloc_distances_s *</type>
      <name>hwloc_get_distance_matrix_covering_obj_by_depth</name>
      <anchorfile>a00063.html</anchorfile>
      <anchor>ga186ac2711b98a12cf46d58cd005fbb51</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj, unsigned depth, unsigned *firstp)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_get_latency</name>
      <anchorfile>a00063.html</anchorfile>
      <anchor>ga9addde99e34b815df47625f1d6191839</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj1, hwloc_obj_t obj2, float *latency, float *reverse_latency)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_advanced_io</name>
    <title>Advanced I/O object traversal helpers</title>
    <filename>a00064.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_non_io_ancestor_obj</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>ga3603275746a8792e54415d79763aa9e9</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_t ioobj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_pcidev</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>gad6e1ed122ef3b6e098538d75acd5e3f6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_pcidev_by_busid</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>ga546e1d690c63fb24177f3013ed78ceb1</anchor>
      <arglist>(hwloc_topology_t topology, unsigned domain, unsigned bus, unsigned dev, unsigned func)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_pcidev_by_busidstring</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>ga9d5643f2e337fe1b98e7cce5c1ecb74e</anchor>
      <arglist>(hwloc_topology_t topology, const char *busid)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_osdev</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>ga73a5bc6265642e6001f7a10812ab886d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_next_bridge</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>ga2bd3f856d0fc3c6c02642c17d763b823</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_bridge_covers_pcibus</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>gaef22d6bf5cb0b7f13a863780126a8cb7</anchor>
      <arglist>(hwloc_obj_t bridge, unsigned domain, unsigned bus)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_get_hostbridge_by_pcibus</name>
      <anchorfile>a00064.html</anchorfile>
      <anchor>ga81d4f68afbd68d94eb70d4f5c603eff5</anchor>
      <arglist>(hwloc_topology_t topology, unsigned domain, unsigned bus)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_bitmap</name>
    <title>The bitmap API</title>
    <filename>a00065.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>hwloc_bitmap_foreach_begin</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga3f6861045a8029ade373510ffa727d2a</anchor>
      <arglist>(id, bitmap)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>hwloc_bitmap_foreach_end</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gafcf3246db406218d4e155735b3fa6528</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>struct hwloc_bitmap_s *</type>
      <name>hwloc_bitmap_t</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gaa3c2bf4c776d603dcebbb61b0c923d84</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct hwloc_bitmap_s *</type>
      <name>hwloc_const_bitmap_t</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga2fb1bbc8aea1ea22dee2f0fd39659f48</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_bitmap_t</type>
      <name>hwloc_bitmap_alloc</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga9fb6aedc30d4ef4998fa53e2403b6130</anchor>
      <arglist>(void) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_bitmap_t</type>
      <name>hwloc_bitmap_alloc_full</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gadd4479485685bda179f4b8b9b70f671e</anchor>
      <arglist>(void) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_free</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga8e7035fe555ef96921bfb98e08519bc7</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC hwloc_bitmap_t</type>
      <name>hwloc_bitmap_dup</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gaaa4ed76211cd3694dfbea2109fc440be</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_copy</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gab14743355fa03b36cef521cbcd2fbf64</anchor>
      <arglist>(hwloc_bitmap_t dst, hwloc_const_bitmap_t src)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_snprintf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga6c0d60db98f27276752c1e5fafcbd7d1</anchor>
      <arglist>(char *__hwloc_restrict buf, size_t buflen, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_asprintf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gad3cf87ceb58aa91656756bbb58057320</anchor>
      <arglist>(char **strp, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_sscanf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga553cd5ceb1925bfd31788e70b4a46ae8</anchor>
      <arglist>(hwloc_bitmap_t bitmap, const char *__hwloc_restrict string)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_list_snprintf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gab94b52560ea4333663895b3276e89af2</anchor>
      <arglist>(char *__hwloc_restrict buf, size_t buflen, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_list_asprintf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga600651f00d39d378f423acbd75d938d2</anchor>
      <arglist>(char **strp, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_list_sscanf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gafe8fc90e7a9171e287cf1bfa11e85980</anchor>
      <arglist>(hwloc_bitmap_t bitmap, const char *__hwloc_restrict string)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_taskset_snprintf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gae0e0619f43a4af5efafb75f489c1c6ec</anchor>
      <arglist>(char *__hwloc_restrict buf, size_t buflen, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_taskset_asprintf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gaa3df92e204ffb24ceddd44d8d58f6eaa</anchor>
      <arglist>(char **strp, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_taskset_sscanf</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga71b28ef0d7206e37adbb94468c03e1a9</anchor>
      <arglist>(hwloc_bitmap_t bitmap, const char *__hwloc_restrict string)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_zero</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga6c540b9fe63b8223b6aba46d56dd63b8</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_fill</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga52456f7ef79d68e610cb65e3f7ffafad</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_only</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga8ea10f5ebaadb8418f28f953d6106190</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_allbut</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga88204c1c313aa067da10241b5886a166</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_from_ulong</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga02eae57cff32b6aa55044e6609ae7d73</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_from_ith_ulong</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga3c44c943cb3a3e4577b32d924e4f36f2</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned i, unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_set</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga497556af0cc34f109ae0277999c074d3</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_set_range</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga9d4f8fc40f552a76c943b5b773ac17d6</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned begin, int end)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_set_ith_ulong</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga56027d6b141a3da441925e6e999de1c1</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned i, unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_clr</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga54e89b87ba5f5c18323f16690e0e5730</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_clr_range</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gac27bd46fd5b387def6b40712b7ee2796</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned begin, int end)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_singlify</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga4630aa1b7e08eac5b41be126194e84a1</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC unsigned long</type>
      <name>hwloc_bitmap_to_ulong</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga4fee4e3cea99e4a389c54a6c3f0c78ed</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC unsigned long</type>
      <name>hwloc_bitmap_to_ith_ulong</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gabc0b9f4cae56a1303c38b2380086efc2</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap, unsigned i) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_isset</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga2583f44cbdb5fff2ea40efdcf3975d3f</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap, unsigned id) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_iszero</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gaa94fed35d2a598bc4a8657b6955b7bf5</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_isfull</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga59a07ecd8e74f2d4ee4271a1dbf0e4dd</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_first</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga42472a7b75ca79cae19f0611fdb849e6</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_next</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga00fc0b9d7d603e271b75956a7dd28826</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap, int prev) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_last</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga4d5786ea9561d9ddbf90bc0ce606c549</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_weight</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga12d520387be74f849f191d7a06ac325c</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_or</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga1ba1de709ee9a7cf5cc8ad2d9a1a81d4</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_and</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gac2f7675dcf1b664a832413d9990d1bcf</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_andnot</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga7618a962685d95564564ba07e9b2199b</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_xor</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga5743af61c30758df75aec29c12444616</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC void</type>
      <name>hwloc_bitmap_not</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga4a8088d62755a083ffce9d592c9e3878</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_intersects</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga575c27953709a8cb9a047aae65157526</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_isincluded</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gaae29e14a926c198e8f91e6e4790621e7</anchor>
      <arglist>(hwloc_const_bitmap_t sub_bitmap, hwloc_const_bitmap_t super_bitmap) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_isequal</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>ga4dd6a75ab63d33ef33bd626b0e489388</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_compare_first</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gac1cbd1e03f9986552243761e657e1752</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2) __hwloc_attribute_pure</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_bitmap_compare</name>
      <anchorfile>a00065.html</anchorfile>
      <anchor>gadd3aa325f2d6a17b5aa3b3be9c740da0</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2) __hwloc_attribute_pure</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_glibc_sched</name>
    <title>Helpers for manipulating glibc sched affinity</title>
    <filename>a00066.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_to_glibc_sched_affinity</name>
      <anchorfile>a00066.html</anchorfile>
      <anchor>ga39454e6013441d32e58ef4c4fcba7e4b</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_const_cpuset_t hwlocset, cpu_set_t *schedset, size_t schedsetsize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_from_glibc_sched_affinity</name>
      <anchorfile>a00066.html</anchorfile>
      <anchor>ga6df504b2f5440b527be05cdad6b1655e</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_cpuset_t hwlocset, const cpu_set_t *schedset, size_t schedsetsize)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux</name>
    <title>Linux-only helpers</title>
    <filename>a00067.html</filename>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_linux_parse_cpumap_file</name>
      <anchorfile>a00067.html</anchorfile>
      <anchor>gaeacad897c30dbea284948374ad4b010c</anchor>
      <arglist>(FILE *file, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_linux_set_tid_cpubind</name>
      <anchorfile>a00067.html</anchorfile>
      <anchor>gaaaca5d1687053b6c3326b2c165bd6530</anchor>
      <arglist>(hwloc_topology_t topology, pid_t tid, hwloc_const_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>HWLOC_DECLSPEC int</type>
      <name>hwloc_linux_get_tid_cpubind</name>
      <anchorfile>a00067.html</anchorfile>
      <anchor>gaf36a9211a21eb930f59090eb5d460b8e</anchor>
      <arglist>(hwloc_topology_t topology, pid_t tid, hwloc_cpuset_t set)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux_libnuma_ulongs</name>
    <title>Helpers for manipulating Linux libnuma unsigned long masks</title>
    <filename>a00068.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_to_linux_libnuma_ulongs</name>
      <anchorfile>a00068.html</anchorfile>
      <anchor>ga018e57a42a780ce2ba2e35ef975d8888</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, unsigned long *mask, unsigned long *maxnode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_nodeset_to_linux_libnuma_ulongs</name>
      <anchorfile>a00068.html</anchorfile>
      <anchor>gaf213df50d229c5d17a5a56b5d8f10b74</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, unsigned long *mask, unsigned long *maxnode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_from_linux_libnuma_ulongs</name>
      <anchorfile>a00068.html</anchorfile>
      <anchor>gafa60816dde33d69149497bcf6c7818e0</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const unsigned long *mask, unsigned long maxnode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_nodeset_from_linux_libnuma_ulongs</name>
      <anchorfile>a00068.html</anchorfile>
      <anchor>ga5b51a4a96a0c3ad9f4c0da308bbf4d8f</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, const unsigned long *mask, unsigned long maxnode)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux_libnuma_bitmask</name>
    <title>Helpers for manipulating Linux libnuma bitmask</title>
    <filename>a00069.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline struct bitmask *</type>
      <name>hwloc_cpuset_to_linux_libnuma_bitmask</name>
      <anchorfile>a00069.html</anchorfile>
      <anchor>ga1a2ad777b9b4cf4aac0123f61c3ab8bd</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline struct bitmask *</type>
      <name>hwloc_nodeset_to_linux_libnuma_bitmask</name>
      <anchorfile>a00069.html</anchorfile>
      <anchor>gab67a395c48ddb56307b1465fab9bb9e6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset) __hwloc_attribute_malloc</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_from_linux_libnuma_bitmask</name>
      <anchorfile>a00069.html</anchorfile>
      <anchor>ga47747968f12c2674d2840dfbacce4940</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const struct bitmask *bitmask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_nodeset_from_linux_libnuma_bitmask</name>
      <anchorfile>a00069.html</anchorfile>
      <anchor>ga5a1d17375cc7b65cd155330008cbdb6a</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, const struct bitmask *bitmask)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux_libnuma_nodemask</name>
    <title>Helpers for manipulating Linux libnuma nodemask_t</title>
    <filename>a00070.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_to_linux_libnuma_nodemask</name>
      <anchorfile>a00070.html</anchorfile>
      <anchor>ga36feb81315de87ce11d9a5aa2b4c6e6d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, nodemask_t *nodemask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_nodeset_to_linux_libnuma_nodemask</name>
      <anchorfile>a00070.html</anchorfile>
      <anchor>gafa4e7cc68f0f99724f8d99541d9ab3de</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, nodemask_t *nodemask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cpuset_from_linux_libnuma_nodemask</name>
      <anchorfile>a00070.html</anchorfile>
      <anchor>gac24c9e4bb2eab3e23b2039559adc9df8</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const nodemask_t *nodemask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_nodeset_from_linux_libnuma_nodemask</name>
      <anchorfile>a00070.html</anchorfile>
      <anchor>ga22862f485346ae3b2ab7d052f538fc5f</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, const nodemask_t *nodemask)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_cuda</name>
    <title>CUDA Driver API Specific Functions</title>
    <filename>a00071.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cuda_get_device_pci_ids</name>
      <anchorfile>a00071.html</anchorfile>
      <anchor>gae45c92fbc7ac538cf69fdfcc73994ed9</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, CUdevice cudevice, int *domain, int *bus, int *dev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cuda_get_device_cpuset</name>
      <anchorfile>a00071.html</anchorfile>
      <anchor>gae06cf330d2f0d9949feb52b146b7d136</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, CUdevice cudevice, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_cuda_get_device_pcidev</name>
      <anchorfile>a00071.html</anchorfile>
      <anchor>ga04d1f8bd566a7f0b30d5e498f9228042</anchor>
      <arglist>(hwloc_topology_t topology, CUdevice cudevice)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_cudart</name>
    <title>CUDA Runtime API Specific Functions</title>
    <filename>a00072.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cudart_get_device_pci_ids</name>
      <anchorfile>a00072.html</anchorfile>
      <anchor>ga1cbf127459986c345f873e2752ddf681</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, int device, int *domain, int *bus, int *dev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_cudart_get_device_cpuset</name>
      <anchorfile>a00072.html</anchorfile>
      <anchor>ga2daaf1dd1a9a7f11ccbc6821374120e9</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, int device, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline hwloc_obj_t</type>
      <name>hwloc_cudart_get_device_pcidev</name>
      <anchorfile>a00072.html</anchorfile>
      <anchor>gaaff84cc4ee4b27f35b21c33626651da0</anchor>
      <arglist>(hwloc_topology_t topology, int device)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_openfabrics</name>
    <title>OpenFabrics-Specific Functions</title>
    <filename>a00073.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_ibv_get_device_cpuset</name>
      <anchorfile>a00073.html</anchorfile>
      <anchor>gaa8ea979ce3a9b8c70ae80bc5716a0fbe</anchor>
      <arglist>(hwloc_topology_t topology __hwloc_attribute_unused, struct ibv_device *ibdev, hwloc_cpuset_t set)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_myriexpress</name>
    <title>Myrinet Express-Specific Functions</title>
    <filename>a00074.html</filename>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_mx_board_get_device_cpuset</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga87f4746d0cad579ce4e86b98088f22a1</anchor>
      <arglist>(hwloc_topology_t topology, unsigned id, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __hwloc_inline int</type>
      <name>hwloc_mx_endpoint_get_device_cpuset</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga2ff7763b9c2c5975f9a2e3d638351535</anchor>
      <arglist>(hwloc_topology_t topology, mx_endpoint_t endpoint, hwloc_cpuset_t set)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_distances_s</name>
    <filename>a00014.html</filename>
    <member kind="variable">
      <type>unsigned</type>
      <name>relative_depth</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a6fe066eaf62ee448aa05bab8e7217ff7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>nbobjs</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a4ca2af858cebbce7324ec49903d09474</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float *</type>
      <name>latency</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a0f70f48d1bfb18e5e2008825da2967c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>latency_max</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>aab61bd1d1ae2e121bfe793c973ec829e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>latency_base</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a204416418049a272bfb51602fc676342</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj</name>
    <filename>a00016.html</filename>
    <member kind="variable">
      <type>hwloc_obj_type_t</type>
      <name>type</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>acc4f0803f244867e68fe0036800be5de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>os_index</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a61a7a80a68eaccbaaa28269e678c81a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>abb709ec38f2970677e4e57d1d30be96d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_memory_s</type>
      <name>memory</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a1dc830816716213b5f797e4052487864</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_obj_attr_u *</type>
      <name>attr</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>accd40e29f71f19e88db62ea3df02adc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a9d82690370275d42d652eccdea5d3ee5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>logical_index</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a0d07fb7b8935e137c94d75a3eb492ae9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>signed</type>
      <name>os_level</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a68766f0b1c4d61b5bad87e3b81dacfde</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>next_cousin</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a85a788017457129589318b6c39451acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>prev_cousin</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ac715989f55ff5a0eb6be2969ee477ec0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>parent</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>adc494f6aed939992be1c55cca5822900</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>sibling_rank</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aaa6043eee6f55869933c1d974efd9acd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>next_sibling</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a7f2343ed476fe4942e6fffd4cade1b40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>prev_sibling</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a7b89e8c189876c0158a9282aaaf17f50</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>arity</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aac3f6da35c9b57599909a44ce2b716c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj **</type>
      <name>children</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a04d05403da37bfe17cd63b7c7dd07b1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>first_child</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>af51d08a0a79dba517c06c5afedc8d2dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>last_child</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a84bd65634dbc55f4158b74443a9bd04f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>userdata</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a76fd3ac94401cf32dfccc3a3a8de68a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>cpuset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a67925e0f2c47f50408fbdb9bddd0790f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>complete_cpuset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a91788a9da687beb7224cc1fd7b75208c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>online_cpuset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a8842d56c2975380f327ea401c5f53564</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>allowed_cpuset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>afa3c59a6dd3da8ffa48710780a1bfb34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_nodeset_t</type>
      <name>nodeset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a08f0d0e16c619a6e653526cbee4ffea3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_nodeset_t</type>
      <name>complete_nodeset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ac38c4012127525ef74c5615c526f4c2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_nodeset_t</type>
      <name>allowed_nodeset</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a19e3d0a5951a7510fc4fc4722a9bf531</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_distances_s **</type>
      <name>distances</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a9a5dd75596edc48fe834f81988cdc0d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>distances_count</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a8be6f63eca4da91000c832280db927b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_info_s *</type>
      <name>infos</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ad59bcd73a10260a0e239be5c4b5888bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>infos_count</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a9843acc28cfbba903b63ea14b137ff70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>symmetric_subtree</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a0f41a1d67dc6b661906f2217563637f3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>hwloc_obj_attr_u</name>
    <filename>a00017.html</filename>
    <class kind="struct">hwloc_obj_attr_u::hwloc_bridge_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_cache_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_group_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_osdev_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_pcidev_attr_s</class>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_cache_attr_s</type>
      <name>cache</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ab5a8ae3bf490e6b1071fea53f7382836</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_group_attr_s</type>
      <name>group</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ae4ba157cc313e2cdd9a82f1c1df7aaa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_pcidev_attr_s</type>
      <name>pcidev</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>a4203d713ce0f5beaa6ee0e9bdac70828</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_bridge_attr_s</type>
      <name>bridge</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>adbdf280699dd84c7619cd8d9edc0d958</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_osdev_attr_s</type>
      <name>osdev</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>a22904c25fe44b323bab5c9bc52660fca</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_bridge_attr_s</name>
    <filename>a00012.html</filename>
    <member kind="variable">
      <type>union hwloc_obj_attr_u::hwloc_bridge_attr_s::@0</type>
      <name>upstream</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a00ce9d99fc8792d1044fe25dc58605fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_pcidev_attr_s</type>
      <name>pci</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>afbfb960b5aaf2b6e10d8d5d424b227d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_obj_bridge_type_t</type>
      <name>upstream_type</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a265dd2164aa2df4972e25a029da72125</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_obj_attr_u::hwloc_bridge_attr_s::@1</type>
      <name>downstream</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>acaf1ae02e37182bbb6966f8c4f35e499</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_bridge_attr_s::@1::@2</type>
      <name>pci</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>af93b3f8c762d7a849b968cb3caf58365</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>domain</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>afcdd4077c7c574e4791eefbdf8fb5d9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>secondary_bus</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>a5e2836eb053bf87e3f7430b5470dedca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>subordinate_bus</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>aae5989d9a424dd74ff22da89e6ed245b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_obj_bridge_type_t</type>
      <name>downstream_type</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ac6a169b672d0e9f75756fd5665828b93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a336c8b22893d5d734d8c9dfca4066b46</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_cache_attr_s</name>
    <filename>a00013.html</filename>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>size</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>abe5e788943ed04302976740c829674c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a5c8f7f39193736c2187ed626940835d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>linesize</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a801e8a668e28caf06c8b88e9ae5c10db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>associativity</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ad06525e474d1e2d1423ed71bb998592d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_group_attr_s</name>
    <filename>a00015.html</filename>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ad914eac61c77481e1b7037877bcc5579</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_osdev_attr_s</name>
    <filename>a00021.html</filename>
    <member kind="variable">
      <type>hwloc_obj_osdev_type_t</type>
      <name>type</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a31e019e27e54ac6138d04be639bb96f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_pcidev_attr_s</name>
    <filename>a00022.html</filename>
    <member kind="variable">
      <type>unsigned short</type>
      <name>domain</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a8fba44988deb98613c1505a4019a34dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>bus</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>aae99e035e8d1387d7b8768aaa8eceb0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>dev</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a3d70c84a12f7e93d14c8d47bf4fd9dc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>func</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a695f32df53f4ef728670bfcf31b74e0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>class_id</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>aee735352d9f1686fd290fc6d6397c334</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>vendor_id</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>aad970ad19b62eb2d5df30e4802da4f4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>device_id</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a35b66064ab7d768caf7154e410caf0fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>subvendor_id</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a499db3d8cc89eaba04fcaef3df1cba97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>subdevice_id</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>acac741aecd7a6db64f33409427e3971f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>revision</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a13ad54c93d08d8ac808e4de9674c5ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>linkspeed</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a59b2fce35f7cbde86c4fd305d0ccda5f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_info_s</name>
    <filename>a00018.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>a115dde58f40338a2d3276d59c099857d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>a522efd7f5262a4493a73fa3476a041ce</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_memory_s</name>
    <filename>a00020.html</filename>
    <class kind="struct">hwloc_obj_memory_s::hwloc_obj_memory_page_type_s</class>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>total_memory</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>a8befd0b3f4f8b695cafed04b31f36a44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>local_memory</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>aace044c7f3fd9dffe50202296bf1dc5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>page_types_len</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>a208c27f4491077d7fb9ba5db8b29cb57</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_memory_s::hwloc_obj_memory_page_type_s *</type>
      <name>page_types</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>a865eba7b12b986d72dbe7a2cfd97c50d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_memory_s::hwloc_obj_memory_page_type_s</name>
    <filename>a00019.html</filename>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>a3f1d261766352288791512974665ce01</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>count</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>aa5b9fa36f1dc47ff3c345fe4dd06b260</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_cpubind_support</name>
    <filename>a00023.html</filename>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisproc_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a9403d51657a4d546b3ea9553a2973a27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisproc_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a77a09ddd78ee3e9ff5f532a6ac74f7eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_proc_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>aa166223d1c2a6de7256ab2d8b675a87e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_proc_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>aae705bc447adc163ead377362c4dfe9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisthread_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a57a89a4b5f1f74fa6cfe176f1e8b0798</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisthread_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a80d762e532d677dff262d83cc7bb1c60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thread_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a46fba33e307909ce256624687799dd6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thread_cpubind</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a8dd4d8531ed2eebdce1507e7d104154e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisproc_last_cpu_location</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ad2d5c94b738d92b7d2ede87e4e96321d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_proc_last_cpu_location</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a2554620148b7992a5093f338f5ae254f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisthread_last_cpu_location</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a6be1f042fdce6bf41b4ea39f6f193808</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_discovery_support</name>
    <filename>a00024.html</filename>
    <member kind="variable">
      <type>unsigned char</type>
      <name>pu</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ad7bb4ecf7a82f5a04fc632e9592ad3ab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_membind_support</name>
    <filename>a00025.html</filename>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisproc_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a36b3e388df9c6a249427cab7e3724749</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisproc_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a3fd51e6fa5f0dd800322301b46b08559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_proc_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a756f44912894b176bf979a1b65f12aac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_proc_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a9880cd2d605e316fc020167c49ca69ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisthread_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a0697af2e41f2e82b8ce71e3cc13f7eac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisthread_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a63b0b2e26157b472f5717ee93cc7c535</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_area_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a476c06f96b65c08b287cf2369966123b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_area_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a0a84e24a06f2fa487fe8c9605c6f68b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>alloc_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ae551abb27d2aa9ce008583488b845b98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>firsttouch_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a221098c339dbfab27bd2c9f5d32f123b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>bind_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ae7cdb1f1b5f0242a69f85b5a5538c764</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>interleave_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a3c44c6012860bbeba8a0f4c19710858d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>replicate_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>afe608fb85eb2aa9706221ccb5fc4d415</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>nexttouch_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ab0921af6e0cd6975812a80b8e5c7435c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>migrate_membind</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>aafa7683871a6a760246f9b35209caec5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_support</name>
    <filename>a00026.html</filename>
    <member kind="variable">
      <type>struct hwloc_topology_discovery_support *</type>
      <name>discovery</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>aea3fbd7653d987d81f848636c420504d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_cpubind_support *</type>
      <name>cpubind</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>adef2bb91f74c3e70a2a071393caf5f56</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_membind_support *</type>
      <name>membind</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ac6eb62ae8bc0a68dce679a7107a36194</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Hardware Locality</title>
    <filename>index</filename>
    <docanchor file="index">cli_examples</docanchor>
    <docanchor file="index">further_read</docanchor>
    <docanchor file="index">Introduction</docanchor>
    <docanchor file="index">installation</docanchor>
    <docanchor file="index">interface_example</docanchor>
    <docanchor file="index">interface</docanchor>
    <docanchor file="index">portability</docanchor>
    <docanchor file="index">history</docanchor>
    <docanchor file="index">bugs</docanchor>
  </compound>
</tagfile>
