<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>termsanddefs</name>
    <title>Terms and Definitions</title>
    <filename>a00002</filename>
  </compound>
  <compound kind="page">
    <name>tools</name>
    <title>Command-Line Tools</title>
    <filename>a00004</filename>
    <docanchor file="a00004" title="lstopo and lstopo-no-graphics">cli_lstopo</docanchor>
    <docanchor file="a00004" title="hwloc-bind">cli_hwloc_bind</docanchor>
    <docanchor file="a00004" title="hwloc-calc">cli_hwloc_calc</docanchor>
    <docanchor file="a00004" title="hwloc-info">cli_hwloc_info</docanchor>
    <docanchor file="a00004" title="hwloc-distrib">cli_hwloc_distrib</docanchor>
    <docanchor file="a00004" title="hwloc-ps">cli_hwloc_ps</docanchor>
    <docanchor file="a00004" title="hwloc-gather-topology">cli_hwloc_gather</docanchor>
    <docanchor file="a00004" title="hwloc-distances">cli_hwloc_distances</docanchor>
    <docanchor file="a00004" title="hwloc-annotate">cli_hwloc_annotate</docanchor>
    <docanchor file="a00004" title="hwloc-diff and hwloc-patch">cli_hwloc_diffpatch</docanchor>
    <docanchor file="a00004" title="hwloc-compress-dir">cli_hwloc_compressdir</docanchor>
    <docanchor file="a00004" title="hwloc-assembler">cli_hwloc_assembler</docanchor>
    <docanchor file="a00004" title="hwloc-assembler-remote">cli_hwloc_assembler_remote</docanchor>
  </compound>
  <compound kind="page">
    <name>envvar</name>
    <title>Environment Variables</title>
    <filename>a00006</filename>
  </compound>
  <compound kind="page">
    <name>cpu_mem_bind</name>
    <title>CPU and Memory Binding Overview</title>
    <filename>a00008</filename>
  </compound>
  <compound kind="page">
    <name>iodevices</name>
    <title>I/O Devices</title>
    <filename>a00010</filename>
    <docanchor file="a00010" title="Enabling and requirements">iodevices_enabling</docanchor>
    <docanchor file="a00010" title="I/O object hierarchy">iodevices_hierarchy</docanchor>
    <docanchor file="a00010" title="Software devices">iodevices_osdev</docanchor>
    <docanchor file="a00010" title="Consulting I/O devices and binding">iodevices_consult</docanchor>
    <docanchor file="a00010" title="Examples">iodevices_examples</docanchor>
  </compound>
  <compound kind="page">
    <name>multinode</name>
    <title>Multi-node Topologies</title>
    <filename>a00012</filename>
    <docanchor file="a00012" title="Multi-node Objects Specifities">multinode_cpusets</docanchor>
    <docanchor file="a00012" title="Assembling topologies with command-line tools">multinode_tools</docanchor>
    <docanchor file="a00012" title="Assembling topologies with the programming interface">multinode_interface</docanchor>
    <docanchor file="a00012" title="Example of assembly with the programming interface">multinode_example</docanchor>
  </compound>
  <compound kind="page">
    <name>attributes</name>
    <title>Object attributes</title>
    <filename>a00014</filename>
    <docanchor file="a00014" title="Normal attributes">attributes_normal</docanchor>
    <docanchor file="a00014" title="Custom string infos">attributes_info</docanchor>
  </compound>
  <compound kind="page">
    <name>xml</name>
    <title>Importing and exporting topologies from/to XML files</title>
    <filename>a00016</filename>
    <docanchor file="a00016" title="libxml2 and minimalistic XML backends">xml_backends</docanchor>
    <docanchor file="a00016" title="XML import error management">xml_errors</docanchor>
  </compound>
  <compound kind="page">
    <name>synthetic</name>
    <title>Synthetic topologies</title>
    <filename>a00018</filename>
    <docanchor file="a00018" title="Synthetic description string">synthetic_string</docanchor>
    <docanchor file="a00018" title="Loading a synthetic topology">synthetic_use</docanchor>
    <docanchor file="a00018" title="Exporting a topology as a synthetic string">synthetic_export</docanchor>
  </compound>
  <compound kind="page">
    <name>interoperability</name>
    <title>Interoperability With Other Software</title>
    <filename>a00020</filename>
  </compound>
  <compound kind="page">
    <name>threadsafety</name>
    <title>Thread Safety</title>
    <filename>a00022</filename>
  </compound>
  <compound kind="page">
    <name>plugins</name>
    <title>Components and plugins</title>
    <filename>a00024</filename>
    <docanchor file="a00024" title="Components enabled by default">plugins_default</docanchor>
    <docanchor file="a00024" title="Selecting which components to use">plugins_select</docanchor>
    <docanchor file="a00024" title="Loading components from plugins">plugins_load</docanchor>
    <docanchor file="a00024" title="Adding new discovery components and plugins">plugins_adding</docanchor>
    <docanchor file="a00024" title="Basics of discovery components">plugins_disc_basic</docanchor>
    <docanchor file="a00024" title="Registering a new discovery component">plugins_disc_register</docanchor>
    <docanchor file="a00024" title="Existing components and plugins">plugins_list</docanchor>
  </compound>
  <compound kind="page">
    <name>embed</name>
    <title>Embedding hwloc in Other Software</title>
    <filename>a00026</filename>
    <docanchor file="a00026" title="Using hwloc&apos;s M4 Embedding Capabilities">embedding_m4</docanchor>
    <docanchor file="a00026" title="Example Embedding hwloc">embedding_example</docanchor>
  </compound>
  <compound kind="page">
    <name>faq</name>
    <title>Frequently Asked Questions</title>
    <filename>a00028</filename>
    <docanchor file="a00028" title="I do not want hwloc to rediscover my enormous machine topology every time I rerun a process">faq_xml</docanchor>
    <docanchor file="a00028" title="How to avoid memory waste when manipulating multiple similar topologies?">faq_diff</docanchor>
    <docanchor file="a00028" title="Why is lstopo slow?">faq_slow_lstopo</docanchor>
    <docanchor file="a00028" title="Does hwloc require privileged access?">faq_privileged</docanchor>
    <docanchor file="a00028" title="hwloc only has a one-dimensional view of the architecture, it ignores distances">faq_onedim</docanchor>
    <docanchor file="a00028" title="How may I ignore symmetric multithreading, hyper-threading, ... ?">faq_smt</docanchor>
    <docanchor file="a00028" title="What happens if my topology is asymmetric?">faq_asymmetric</docanchor>
    <docanchor file="a00028" title="How do I annotate the topology with private notes?">faq_annotate</docanchor>
    <docanchor file="a00028" title="Why does Valgrind complain about hwloc memory leaks?">faq_valgrind</docanchor>
    <docanchor file="a00028" title="How do I handle API upgrades?">faq_upgrade</docanchor>
    <docanchor file="a00028" title="How do I build hwloc for BlueGene/Q?">faq_bgq</docanchor>
    <docanchor file="a00028" title="How to get useful topology information on NetBSD?">faq_netbsd_bind</docanchor>
  </compound>
  <compound kind="group">
    <name>hwlocality_api_version</name>
    <title>API version</title>
    <filename>a00072.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>HWLOC_API_VERSION</name>
      <anchorfile>a00072.html</anchorfile>
      <anchor>ga8f4dfb8eef138af55dd1a0fa802e5476</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HWLOC_COMPONENT_ABI</name>
      <anchorfile>a00072.html</anchorfile>
      <anchor>gaac5bc1f46f55e10ef0141a68ce70e21f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>unsigned</type>
      <name>hwloc_get_api_version</name>
      <anchorfile>a00072.html</anchorfile>
      <anchor>ga9c0b50c98add1adf57ed1ce85bb5190d</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_object_sets</name>
    <title>Object Sets (hwloc_cpuset_t and hwloc_nodeset_t)</title>
    <filename>a00073.html</filename>
    <member kind="typedef">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_cpuset_t</name>
      <anchorfile>a00073.html</anchorfile>
      <anchor>ga4bbf39b68b6f568fb92739e7c0ea7801</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>hwloc_const_bitmap_t</type>
      <name>hwloc_const_cpuset_t</name>
      <anchorfile>a00073.html</anchorfile>
      <anchor>ga1f784433e9b606261f62d1134f6a3b25</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_nodeset_t</name>
      <anchorfile>a00073.html</anchorfile>
      <anchor>ga37e35730fa7e775b5bb0afe893d6d508</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>hwloc_const_bitmap_t</type>
      <name>hwloc_const_nodeset_t</name>
      <anchorfile>a00073.html</anchorfile>
      <anchor>ga2f5276235841ad66a79bedad16a5a10c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_object_types</name>
    <title>Object Types</title>
    <filename>a00074.html</filename>
    <member kind="typedef">
      <type>enum hwloc_obj_cache_type_e</type>
      <name>hwloc_obj_cache_type_t</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gab6e1e7efedae8b341f3ee14fbe53d66c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum hwloc_obj_bridge_type_e</type>
      <name>hwloc_obj_bridge_type_t</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga0a947e8c5adcc729b126bd09c01a0153</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum hwloc_obj_osdev_type_e</type>
      <name>hwloc_obj_osdev_type_t</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga90c1e82a60ba5871d07645169e636987</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_obj_type_t</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gacd37bb612667dc437d66bfb175a8dc55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_SYSTEM</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a3aa1b842d1fd4207ebce171f95a244ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_MACHINE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a3f4e83ffc4a259354959ae8a9eaa2a80</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_NODE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55aaf0964881117bdedf1a5e9332cd120dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_SOCKET</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a1ac6e07775ae4324b3fe9dbd72c785ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_CACHE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a56ee0b7eca88f363b75b34fdde8c9ddc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_CORE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55ac793958f330bca371aa1535de8aff45f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_PU</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55abca6887e80cb291353b0a0c1da83f661</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_GROUP</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a5269ef95be72f88465559d35c9b7ad56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_MISC</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a19f8a6953fa91efc76bcbcdf2d22de4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_BRIDGE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a6825f10895fea60aca7a6ba9fe273db0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_PCI_DEVICE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a5d8117a54df1fbd3606ab19e42cb0ea9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OS_DEVICE</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55a51e7280240fd9f25589cbbe538bdb075</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_TYPE_MAX</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ggacd37bb612667dc437d66bfb175a8dc55addb5f843e1812445a84e6b2a844b1ebc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_obj_cache_type_e</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga791c9875c8fe20f3e1e5871e0657e59b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_CACHE_UNIFIED</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga791c9875c8fe20f3e1e5871e0657e59ba3900b3b2db54941aac249e5a638a2d7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_CACHE_DATA</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga791c9875c8fe20f3e1e5871e0657e59bacac60ecad4206f85aeb79bef1604b488</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_CACHE_INSTRUCTION</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga791c9875c8fe20f3e1e5871e0657e59ba6f98b0d422b38ba90c5f5c79a11b0658</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_obj_bridge_type_e</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga48a4803c72574191d7ead1c62aaf9860</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_BRIDGE_HOST</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga48a4803c72574191d7ead1c62aaf9860a2c7660f3864ad2810c1e72aad285e574</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_BRIDGE_PCI</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga48a4803c72574191d7ead1c62aaf9860a8f3b4cecf3dab6073d74696d10863c60</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_obj_osdev_type_e</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga64f5d539df299c97ae80ce53fc4b56c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OSDEV_BLOCK</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a689b0488c3c0d08d116751c6b9cb8871</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OSDEV_GPU</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0aa3a09798ef2836abb236dc3a645ffc90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OSDEV_NETWORK</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0ab715d81155f771573c8682dffc65021b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OSDEV_OPENFABRICS</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a52157d03694fdae82dddd57ca8c973b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OSDEV_DMA</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a827ad1643360711a8b6c6af671366791</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_OBJ_OSDEV_COPROC</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga64f5d539df299c97ae80ce53fc4b56c0a46f8927e1c3e137eaa86cc8f6861fb83</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_compare_types_e</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga46323568968005137c32f6a1cd405b74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TYPE_UNORDERED</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>gga46323568968005137c32f6a1cd405b74a2f8297ea36eba46e7596e810a67298fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_compare_types</name>
      <anchorfile>a00074.html</anchorfile>
      <anchor>ga1820ea0dfd8e9dca28f9ea7624df5ae2</anchor>
      <arglist>(hwloc_obj_type_t type1, hwloc_obj_type_t type2)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_objects</name>
    <title>Object Structure and Attributes</title>
    <filename>a00075.html</filename>
    <class kind="struct">hwloc_obj_memory_s</class>
    <class kind="struct">hwloc_obj_memory_s::hwloc_obj_memory_page_type_s</class>
    <class kind="struct">hwloc_obj</class>
    <class kind="union">hwloc_obj_attr_u</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_cache_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_group_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_pcidev_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_bridge_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_osdev_attr_s</class>
    <class kind="struct">hwloc_distances_s</class>
    <class kind="struct">hwloc_obj_info_s</class>
    <member kind="typedef">
      <type>struct hwloc_obj *</type>
      <name>hwloc_obj_t</name>
      <anchorfile>a00075.html</anchorfile>
      <anchor>ga79b8ab56877ef99ac59b833203391c7d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_creation</name>
    <title>Topology Creation and Destruction</title>
    <filename>a00076.html</filename>
    <member kind="typedef">
      <type>struct hwloc_topology *</type>
      <name>hwloc_topology_t</name>
      <anchorfile>a00076.html</anchorfile>
      <anchor>ga9d1e76ee15a7dee158b786c30b6a6e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_init</name>
      <anchorfile>a00076.html</anchorfile>
      <anchor>ga03fd4a16d8b9ee1ffc32b25fd2f6bdfa</anchor>
      <arglist>(hwloc_topology_t *topologyp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_load</name>
      <anchorfile>a00076.html</anchorfile>
      <anchor>gabdf58d87ad77f6615fccdfe0535ff826</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_topology_destroy</name>
      <anchorfile>a00076.html</anchorfile>
      <anchor>ga9f34a640b6fd28d23699d4d084667b15</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_topology_check</name>
      <anchorfile>a00076.html</anchorfile>
      <anchor>gaf6746bc3a558ef1ac8348b4491d091b5</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_configuration</name>
    <title>Topology Detection Configuration and Query</title>
    <filename>a00077.html</filename>
    <class kind="struct">hwloc_topology_discovery_support</class>
    <class kind="struct">hwloc_topology_cpubind_support</class>
    <class kind="struct">hwloc_topology_membind_support</class>
    <class kind="struct">hwloc_topology_support</class>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_topology_flags_e</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>gada025d3ec20b4b420f8038d23d6e7bde</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea129b4fea1300be22bbaf0bb0958994c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea6ecb6abc6a0bb75e81564f8bca85783b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_FLAG_IO_DEVICES</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea46ae25e8896278840b1800ae9ce4de41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_FLAG_IO_BRIDGES</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea426b18c349f15d7046bb391d96fa947c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_FLAG_WHOLE_IO</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdea4a41dc181649ef81c2dcd44a54e271b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_FLAG_ICACHES</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ggada025d3ec20b4b420f8038d23d6e7bdeaae509a0bed4a7067e0116c75c661178d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_ignore_type</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>gafcf30842e8cb47b4c3dcaebecea31e17</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_ignore_type_keep_structure</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga1f987bca941d6949faf7b1554dd7bc12</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_ignore_all_keep_structure</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga7c9cf147442d65d755c664ccde3bb3ef</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_flags</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>gaaeed4df656979e5f16befea9d29b814b</anchor>
      <arglist>(hwloc_topology_t topology, unsigned long flags)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>hwloc_topology_get_flags</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga09318f81c1d4713be907d64748a6f93c</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_pid</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga341fc17b5867a4715570baab131f68cd</anchor>
      <arglist>(hwloc_topology_t restrict topology, hwloc_pid_t pid)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_fsroot</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>gac0ea95d5f2ad78408bc267a0441c7c97</anchor>
      <arglist>(hwloc_topology_t restrict topology, const char *restrict fsroot_path)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_synthetic</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga4fab186bb6181a00bcf585825fddd38d</anchor>
      <arglist>(hwloc_topology_t restrict topology, const char *restrict description)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_xml</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga879439b7ee99407ee911b3ac64e9a25e</anchor>
      <arglist>(hwloc_topology_t restrict topology, const char *restrict xmlpath)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_xmlbuffer</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga2745616b65595e1c1e579ecc7e461fa8</anchor>
      <arglist>(hwloc_topology_t restrict topology, const char *restrict buffer, int size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_custom</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>gad1f15795219e2c3ca6d44ba0db604c4f</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_set_distance_matrix</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga90ed5733946b2e9aba90d2e2c1fbfbd7</anchor>
      <arglist>(hwloc_topology_t restrict topology, hwloc_obj_type_t type, unsigned nbobjs, unsigned *os_index, float *distances)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_is_thissystem</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>ga68ffdcfd9175cdf40709801092f18017</anchor>
      <arglist>(hwloc_topology_t restrict topology)</arglist>
    </member>
    <member kind="function">
      <type>const struct hwloc_topology_support *</type>
      <name>hwloc_topology_get_support</name>
      <anchorfile>a00077.html</anchorfile>
      <anchor>gab8c76173c4a8ce1a9a9366012b1388e6</anchor>
      <arglist>(hwloc_topology_t restrict topology)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_levels</name>
    <title>Object levels, depths and types</title>
    <filename>a00078.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_get_type_depth_e</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>gaf4e663cf42bbe20756b849c6293ef575</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TYPE_DEPTH_UNKNOWN</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575a0565ab92ab72cb0cec91e23003294aad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TYPE_DEPTH_MULTIPLE</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575ae99465995cacde6c210d5fc2e409798c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TYPE_DEPTH_BRIDGE</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575af93b50182973e4a718d9d4db9e253a90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TYPE_DEPTH_PCI_DEVICE</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575ad8b1516e699b57ce1c8d107fbd2f674c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TYPE_DEPTH_OS_DEVICE</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ggaf4e663cf42bbe20756b849c6293ef575afe9a2131073eebbe129d4aa2928d3f46</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>unsigned</type>
      <name>hwloc_topology_get_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>gafa4f8dbc4c2e74c0da8019446353eed1</anchor>
      <arglist>(hwloc_topology_t restrict topology)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_type_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga8bec782e21be313750da70cf7428b374</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_type_or_below_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga8125328e69eba709c33ea8055c12589b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_type_or_above_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga8a9ee573b7d2190272095d10712a7cca</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>hwloc_obj_type_t</type>
      <name>hwloc_get_depth_type</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga8cc04ad9eb03b0b74d420adf8cc11ad2</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth)</arglist>
    </member>
    <member kind="function">
      <type>unsigned</type>
      <name>hwloc_get_nbobjs_by_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>gab17065e3d53455973844568d9f21c72c</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_nbobjs_by_type</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga789a3f65aedff644be64a18526a03065</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_root_obj</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga2d4b12fc187dfc53b35f2fa21d21044d</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function">
      <type>hwloc_obj_t</type>
      <name>hwloc_get_obj_by_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>gabf8a98ad085460a4982cc7b74c344b71</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth, unsigned idx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_obj_by_type</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga6f414dd80a2b943967a0ac92da3181a2</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type, unsigned idx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_obj_by_depth</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga8cf03328ed09901a03664d9498177126</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_obj_by_type</name>
      <anchorfile>a00078.html</anchorfile>
      <anchor>ga759e88eaf5a230ad283e9d4c42486735</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type, hwloc_obj_t prev)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_object_strings</name>
    <title>Manipulating Object Type, Sets and Attributes as Strings</title>
    <filename>a00079.html</filename>
    <member kind="function">
      <type>const char *</type>
      <name>hwloc_obj_type_string</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>ga5ca0bf94bbbb080d0eff17a57bd90422</anchor>
      <arglist>(hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_obj_type_sscanf</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>ga8f8452fa8e707cad59b8a73a6fa06059</anchor>
      <arglist>(const char *string, hwloc_obj_type_t *typep, int *depthattrp, void *typeattrp, size_t typeattrsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_obj_type_snprintf</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>gadb8765c260edea80c52cd06a76639ba4</anchor>
      <arglist>(char *restrict string, size_t size, hwloc_obj_t obj, int verbose)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_obj_attr_snprintf</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>ga870e876931c282a1c7aee2f031912ce3</anchor>
      <arglist>(char *restrict string, size_t size, hwloc_obj_t obj, const char *restrict separator, int verbose)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_obj_cpuset_snprintf</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>ga8e0472dfa655c68c0c9072e1ee3a1e03</anchor>
      <arglist>(char *restrict str, size_t size, size_t nobj, const hwloc_obj_t *restrict objs)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const char *</type>
      <name>hwloc_obj_get_info_by_name</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>gab358661a92bb27d8542b255cc9f6f25e</anchor>
      <arglist>(hwloc_obj_t obj, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_obj_add_info</name>
      <anchorfile>a00079.html</anchorfile>
      <anchor>ga7e90c5398a9d77df31d7d45faf0f316b</anchor>
      <arglist>(hwloc_obj_t obj, const char *name, const char *value)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_cpubinding</name>
    <title>CPU binding</title>
    <filename>a00080.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_cpubind_flags_t</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>ga217dc8d373f8958cc93c154ebce1c71c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_CPUBIND_PROCESS</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71ca2e0dd0128dac6b03408c7dd170477fdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_CPUBIND_THREAD</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71caf1b6bbad00d7b1017b918e3719f4d421</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_CPUBIND_STRICT</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71ca679a7e0f0c7ee06b123565f90d98e7fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_CPUBIND_NOMEMBIND</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gga217dc8d373f8958cc93c154ebce1c71ca41ce440443cc3087caed95ab60edcad6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_cpubind</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>ga80bc07473a8edf840cae17bd7ec21d48</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_cpubind</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gacba7ecb979baf824d5240fa2cb2a8be6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_proc_cpubind</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>ga296db8a3c6d49b51fb83d6f3e45c02a6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_proc_cpubind</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gac82de91f788fa82dacf99c6e0d4b7158</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_thread_cpubind</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gae42c01b2addcfbf6048b9a516dd7a906</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_thread_t thread, hwloc_const_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_thread_cpubind</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gaf13f765642b5d0d9a9813e6bb043671b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_thread_t thread, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_last_cpu_location</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>gafdb374627358bf09203b5a4215b13032</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t set, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_proc_last_cpu_location</name>
      <anchorfile>a00080.html</anchorfile>
      <anchor>ga910a05c2d47f68a3155bf176b50fa555</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t set, int flags)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_membinding</name>
    <title>Memory binding</title>
    <filename>a00081.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_membind_policy_t</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>gac9764f79505775d06407b40f5e4661e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_DEFAULT</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8a18675bb80ebc1bce5b652e9de8f3998c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_FIRSTTOUCH</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8a979c7aa78dd32780858f30f47a72cca0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_BIND</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8ad811fa4b2a6002c4d63695a408ffde2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_INTERLEAVE</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8ae370075e5af016d42310f87ea5af236b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_REPLICATE</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8afb37480fe5f4236eb7dd4aef26f691e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_NEXTTOUCH</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8aecdd4164d647708fbb51a00d98dbb138</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_MIXED</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggac9764f79505775d06407b40f5e4661e8a3185bd869b67817fb2bd5164bf360402</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_membind_flags_t</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>gab00475fd98815bf4fb9aaf752030e7d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_PROCESS</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2a1b1b74aef138f64aff214a8cbdfe8eb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_THREAD</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2a1dc7dd5cdcd5796893a325a524555298</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_STRICT</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2a0335311a0ee04166df2888d52b4a42c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_MIGRATE</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2aa6e49e54f52827cb143cc869cfd748af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_MEMBIND_NOCPUBIND</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ggab00475fd98815bf4fb9aaf752030e7d2aad6b9eaf2ee324ca58dc8f58094b9997</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga5cac0bcbe770b43fc63a6b00ea4a7a9d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga53a493e13ad8e5f924b50c07713ee5df</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga84ee275e84286d17a5853c52272a6a95</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>gabf567d2e3b956591a43de0c1941943b0</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_proc_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga21b042d94266a26a9c82b98c4bb30d01</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_proc_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga94b91473febe49fc7af09a0f4a72e53a</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_proc_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>gac5075b7a2bf55f48f4622351817addad</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_nodeset_t nodeset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_proc_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>gada7b3bb7f278542dbb38dc3d6f527c29</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t cpuset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_area_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga7f11bd709ac0cb93af613e2dd84165ad</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_set_area_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga93d2698ae5d7330be5e9f8ece6bc9b2c</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_area_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga5b674a9e043814bfd1c927fd0137fb19</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_nodeset_t nodeset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_area_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>gaf75f365807447cfdc73e4e04e3469d84</anchor>
      <arglist>(hwloc_topology_t topology, const void *addr, size_t len, hwloc_cpuset_t cpuset, hwloc_membind_policy_t *policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>hwloc_alloc</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga972b335a86a7d5e7b34bce2b243c41bc</anchor>
      <arglist>(hwloc_topology_t topology, size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>hwloc_alloc_membind_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga0ff3076f7f3633637699b809bcceceb1</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>hwloc_alloc_membind</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga1058b0b9216bc6fd9c3734ea9dca1e54</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_cpuset_t cpuset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>hwloc_alloc_membind_policy_nodeset</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga7d473e80f11d774421688e36e9926136</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>hwloc_alloc_membind_policy</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga4f7240065719cf923eb0cc078afcdcc2</anchor>
      <arglist>(hwloc_topology_t topology, size_t len, hwloc_const_cpuset_t set, hwloc_membind_policy_t policy, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_free</name>
      <anchorfile>a00081.html</anchorfile>
      <anchor>ga32dbd4f54e9e4a7179f2dde37ffe6ad7</anchor>
      <arglist>(hwloc_topology_t topology, void *addr, size_t len)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_tinker</name>
    <title>Modifying a loaded Topology</title>
    <filename>a00082.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_restrict_flags_e</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>ga9d80f08eb25b7ac22f1b998dc8bf521f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_RESTRICT_FLAG_ADAPT_DISTANCES</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>gga9d80f08eb25b7ac22f1b998dc8bf521fa4d18407f5520793b50b9e892f5bb55d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_RESTRICT_FLAG_ADAPT_MISC</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>gga9d80f08eb25b7ac22f1b998dc8bf521fa699969227a09bbc1a7de51dc9fb7be4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_RESTRICT_FLAG_ADAPT_IO</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>gga9d80f08eb25b7ac22f1b998dc8bf521faa95d6985e36ec1e55f68b210297a85cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>hwloc_obj_t</type>
      <name>hwloc_topology_insert_misc_object_by_cpuset</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>ga77c753751f3b90fac14117523fe604df</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>hwloc_obj_t</type>
      <name>hwloc_topology_insert_misc_object_by_parent</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>ga1472cdf93327dfff4b5b2aedfde29cf3</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t parent, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_restrict</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>gaffdb5403e5178b0d29852dd3319de3fc</anchor>
      <arglist>(hwloc_topology_t restrict topology, hwloc_const_cpuset_t cpuset, unsigned long flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_dup</name>
      <anchorfile>a00082.html</anchorfile>
      <anchor>ga62a161fc5e6f120344dc69a7bee4e587</anchor>
      <arglist>(hwloc_topology_t *newtopology, hwloc_topology_t oldtopology)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_custom</name>
    <title>Building Custom Topologies</title>
    <filename>a00083.html</filename>
    <member kind="function">
      <type>int</type>
      <name>hwloc_custom_insert_topology</name>
      <anchorfile>a00083.html</anchorfile>
      <anchor>ga58220f648a972140865d11940bccec81</anchor>
      <arglist>(hwloc_topology_t newtopology, hwloc_obj_t newparent, hwloc_topology_t oldtopology, hwloc_obj_t oldroot)</arglist>
    </member>
    <member kind="function">
      <type>hwloc_obj_t</type>
      <name>hwloc_custom_insert_group_object_by_parent</name>
      <anchorfile>a00083.html</anchorfile>
      <anchor>ga60c404d4f0544e1070f549af8be6bd73</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t parent, int groupdepth)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_xmlexport</name>
    <title>Exporting Topologies to XML</title>
    <filename>a00084.html</filename>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_export_xml</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>ga2bf166eda4d3605362ac86f77684002a</anchor>
      <arglist>(hwloc_topology_t topology, const char *xmlpath)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_export_xmlbuffer</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>ga7cbf8a2d8579d192911007650366fc8a</anchor>
      <arglist>(hwloc_topology_t topology, char **xmlbuffer, int *buflen)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_free_xmlbuffer</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>ga293e4a6489f15fd16ad22a5734561cf1</anchor>
      <arglist>(hwloc_topology_t topology, char *xmlbuffer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_topology_set_userdata_export_callback</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>ga9d6ff0f7a8dd45be9aa8575ef31978cc</anchor>
      <arglist>(hwloc_topology_t topology, void(*export_cb)(void *reserved, hwloc_topology_t topology, hwloc_obj_t obj))</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_export_obj_userdata</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>gaa541bdd628416dbbe97d0df69d3de958</anchor>
      <arglist>(void *reserved, hwloc_topology_t topology, hwloc_obj_t obj, const char *name, const void *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_export_obj_userdata_base64</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>ga1b7358137cec27bd073f58a606ecbaba</anchor>
      <arglist>(void *reserved, hwloc_topology_t topology, hwloc_obj_t obj, const char *name, const void *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_topology_set_userdata_import_callback</name>
      <anchorfile>a00084.html</anchorfile>
      <anchor>ga5ac6917ea7289955fb1ffda4353af9b0</anchor>
      <arglist>(hwloc_topology_t topology, void(*import_cb)(hwloc_topology_t topology, hwloc_obj_t obj, const char *name, const void *buffer, size_t length))</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_inside</name>
    <title>Finding Objects inside a CPU set</title>
    <filename>a00085.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_first_largest_obj_inside_cpuset</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga547325a4193dbc215d07c23097519d79</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_get_largest_objs_inside_cpuset</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga34ca563fa3a6a4e05268f36a87668511</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_t *restrict objs, int max)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_obj_inside_cpuset_by_depth</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga468b8bef96f9677d6fe04a0d81044c85</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_obj_inside_cpuset_by_type</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>gafe859176cf9233f215fea0e87cef47e7</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_obj_inside_cpuset_by_depth</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga9442c310da87de6d09d3a4b8399ec14f</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth, unsigned idx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_obj_inside_cpuset_by_type</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga2edf924e5a0cca9cb22f65cc77c6ddb5</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type, unsigned idx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned</type>
      <name>hwloc_get_nbobjs_inside_cpuset_by_depth</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga8874c24e73ba2b04c76ec8b9e54f49f8</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_nbobjs_inside_cpuset_by_type</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga133c40415de008518608f8bff52a5ab9</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_obj_index_inside_cpuset</name>
      <anchorfile>a00085.html</anchorfile>
      <anchor>ga4c3a20d61e9beb06c667b21688c772c5</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_t obj)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_covering</name>
    <title>Finding Objects covering at least CPU set</title>
    <filename>a00086.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_child_covering_cpuset</name>
      <anchorfile>a00086.html</anchorfile>
      <anchor>ga0e66aa2dc6b2527cfd10723af646c9d7</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_t parent)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_obj_covering_cpuset</name>
      <anchorfile>a00086.html</anchorfile>
      <anchor>ga41acdcdbb8b95d70ecf6c572a3f67dca</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_obj_covering_cpuset_by_depth</name>
      <anchorfile>a00086.html</anchorfile>
      <anchor>gaeea0d93522c85c3ef0acfbb00733a69c</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, unsigned depth, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_obj_covering_cpuset_by_type</name>
      <anchorfile>a00086.html</anchorfile>
      <anchor>ga91cfae08b092e7966002e15d1e987213</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set, hwloc_obj_type_t type, hwloc_obj_t prev)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_ancestors</name>
    <title>Looking at Ancestor and Child Objects</title>
    <filename>a00087.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_ancestor_obj_by_depth</name>
      <anchorfile>a00087.html</anchorfile>
      <anchor>gaa367c98dea402386c040a47a2f414f86</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth, hwloc_obj_t obj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_ancestor_obj_by_type</name>
      <anchorfile>a00087.html</anchorfile>
      <anchor>ga70f0c7583291da707c15ae4daa850f41</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type, hwloc_obj_t obj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_common_ancestor_obj</name>
      <anchorfile>a00087.html</anchorfile>
      <anchor>gab1fa883021928b5c476911c4102e9be3</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj1, hwloc_obj_t obj2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_obj_is_in_subtree</name>
      <anchorfile>a00087.html</anchorfile>
      <anchor>ga408cf74f5bf9ed497911a320c7f8cc56</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj, hwloc_obj_t subtree_root)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_child</name>
      <anchorfile>a00087.html</anchorfile>
      <anchor>ga12d8565a3436c565e791ed02a0353621</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t parent, hwloc_obj_t prev)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_cache</name>
    <title>Looking at Cache Objects</title>
    <filename>a00088.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_cache_type_depth</name>
      <anchorfile>a00088.html</anchorfile>
      <anchor>gad108a09ce400222fe45545257d575489</anchor>
      <arglist>(hwloc_topology_t topology, unsigned cachelevel, hwloc_obj_cache_type_t cachetype)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_cache_covering_cpuset</name>
      <anchorfile>a00088.html</anchorfile>
      <anchor>gac4cd480a5deaa4ada047fdd11ae4c070</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_shared_cache_covering_obj</name>
      <anchorfile>a00088.html</anchorfile>
      <anchor>ga36f48c32837c7a70b424706e213dcd71</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_find_misc</name>
    <title>Finding objects, miscellaneous helpers</title>
    <filename>a00089.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_pu_obj_by_os_index</name>
      <anchorfile>a00089.html</anchorfile>
      <anchor>ga751c238a4931db5cc0ca3181b7dd7479</anchor>
      <arglist>(hwloc_topology_t topology, unsigned os_index)</arglist>
    </member>
    <member kind="function">
      <type>unsigned</type>
      <name>hwloc_get_closest_objs</name>
      <anchorfile>a00089.html</anchorfile>
      <anchor>ga2cd22a34360643f7f5bad09576dec205</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t src, hwloc_obj_t *restrict objs, unsigned max)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_obj_below_by_type</name>
      <anchorfile>a00089.html</anchorfile>
      <anchor>ga7a0c1046851f7a88bd52f5a1d4ba0a97</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type1, unsigned idx1, hwloc_obj_type_t type2, unsigned idx2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_obj_below_array_by_type</name>
      <anchorfile>a00089.html</anchorfile>
      <anchor>gacb51295ff3fbd3a96f990f20c6492b1d</anchor>
      <arglist>(hwloc_topology_t topology, int nr, hwloc_obj_type_t *typev, unsigned *idxv)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_distribute</name>
    <title>Distributing items over a topology</title>
    <filename>a00090.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_distrib_flags_e</name>
      <anchorfile>a00090.html</anchorfile>
      <anchor>ga8b835295a52b6768a5e6c8abb1f9c54d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_DISTRIB_FLAG_REVERSE</name>
      <anchorfile>a00090.html</anchorfile>
      <anchor>gga8b835295a52b6768a5e6c8abb1f9c54da2ca08404bfbebe9ed5f34c3d7635425a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_distrib</name>
      <anchorfile>a00090.html</anchorfile>
      <anchor>ga2e3db06f766509ae2004ee4c8bbf5204</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t *roots, unsigned n_roots, hwloc_cpuset_t *set, unsigned n, unsigned until, unsigned long flags)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_topology_sets</name>
    <title>CPU and node sets of entire topologies</title>
    <filename>a00091.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_complete_cpuset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>ga69fbad44c685e46d80ecf51a714ba9ce</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_topology_cpuset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>ga5c3ddcb2d19bc0b58042c9fc974f0fa5</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_online_cpuset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>gacec35bb7656c1944c514044ca4ce9c6f</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_const_cpuset_t</type>
      <name>hwloc_topology_get_allowed_cpuset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>ga8cb598b3bce8d67cbdfc1f29caad28f6</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_const_nodeset_t</type>
      <name>hwloc_topology_get_complete_nodeset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>ga22002d6fc14a25a3947c42cba0a70134</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_const_nodeset_t</type>
      <name>hwloc_topology_get_topology_nodeset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>ga2f1cb5bc87dabb252b691c5c15555d21</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_const_nodeset_t</type>
      <name>hwloc_topology_get_allowed_nodeset</name>
      <anchorfile>a00091.html</anchorfile>
      <anchor>ga706182498ac92528e274e9b8d944273d</anchor>
      <arglist>(hwloc_topology_t topology)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_helper_nodeset_convert</name>
    <title>Converting between CPU sets and node sets</title>
    <filename>a00092.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>hwloc_cpuset_to_nodeset</name>
      <anchorfile>a00092.html</anchorfile>
      <anchor>ga8fbd641f284673a3e884e8e556044eba</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t _cpuset, hwloc_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>hwloc_cpuset_to_nodeset_strict</name>
      <anchorfile>a00092.html</anchorfile>
      <anchor>ga014490779e7012ffec2b5ccee020c0ae</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_const_cpuset_t _cpuset, hwloc_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>hwloc_cpuset_from_nodeset</name>
      <anchorfile>a00092.html</anchorfile>
      <anchor>ga2ed57313ce5bf85f3ac156932ecf639d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t _cpuset, hwloc_const_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>hwloc_cpuset_from_nodeset_strict</name>
      <anchorfile>a00092.html</anchorfile>
      <anchor>gacde800e58b8cdb746706b371ca4b97e0</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_cpuset_t _cpuset, hwloc_const_nodeset_t nodeset)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_distances</name>
    <title>Manipulating Distances</title>
    <filename>a00093.html</filename>
    <member kind="function" static="yes">
      <type>static const struct hwloc_distances_s *</type>
      <name>hwloc_get_whole_distance_matrix_by_depth</name>
      <anchorfile>a00093.html</anchorfile>
      <anchor>ga07219dc0fd6e4553a658eaa73f038c42</anchor>
      <arglist>(hwloc_topology_t topology, unsigned depth)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const struct hwloc_distances_s *</type>
      <name>hwloc_get_whole_distance_matrix_by_type</name>
      <anchorfile>a00093.html</anchorfile>
      <anchor>ga64529254be72b3c4343b25926c84b12f</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_type_t type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const struct hwloc_distances_s *</type>
      <name>hwloc_get_distance_matrix_covering_obj_by_depth</name>
      <anchorfile>a00093.html</anchorfile>
      <anchor>ga2d159b21a49964cc629dde03cbf05702</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj, unsigned depth, unsigned *firstp)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_get_latency</name>
      <anchorfile>a00093.html</anchorfile>
      <anchor>ga79ed880a55f9d03b4ae58ef5116812d7</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t obj1, hwloc_obj_t obj2, float *latency, float *reverse_latency)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_advanced_io</name>
    <title>Finding I/O objects</title>
    <filename>a00094.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_non_io_ancestor_obj</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>gaf139bb61375178e90cc3f1835b452ab6</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t ioobj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_pcidev</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>ga66470dabce9db19a57c5940a909d0baa</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_pcidev_by_busid</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>gacdbaf0db98872e224b7883a84bfb0455</anchor>
      <arglist>(hwloc_topology_t topology, unsigned domain, unsigned bus, unsigned dev, unsigned func)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_pcidev_by_busidstring</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>ga23a978469353134b3104b846fe2efceb</anchor>
      <arglist>(hwloc_topology_t topology, const char *busid)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_osdev</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>ga8b4584c8949e2c5f1c97ba7fe92b8145</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_next_bridge</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>ga9dba22a3f4f701f2a46780ba9a0bbbe7</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t prev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_bridge_covers_pcibus</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>ga0d92a9462a3d317f29ecb4442a307fb1</anchor>
      <arglist>(hwloc_obj_t bridge, unsigned domain, unsigned bus)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_get_hostbridge_by_pcibus</name>
      <anchorfile>a00094.html</anchorfile>
      <anchor>ga06442aedfd6414798ed66a68e4e90e44</anchor>
      <arglist>(hwloc_topology_t topology, unsigned domain, unsigned bus)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_bitmap</name>
    <title>The bitmap API</title>
    <filename>a00095.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>hwloc_bitmap_foreach_begin</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga3f6861045a8029ade373510ffa727d2a</anchor>
      <arglist>(id, bitmap)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>hwloc_bitmap_foreach_end</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gafcf3246db406218d4e155735b3fa6528</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>struct hwloc_bitmap_s *</type>
      <name>hwloc_bitmap_t</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaa3c2bf4c776d603dcebbb61b0c923d84</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>const struct hwloc_bitmap_s *</type>
      <name>hwloc_const_bitmap_t</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gae991a108af01d408be2776c5b2c467b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_bitmap_alloc</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gadece3d1eb5199fc2fb99bc7dcf1ccc05</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_bitmap_alloc_full</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga02853b4a224970587b9d7a0e20d857c0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_free</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga156130d85b3a0674d6e0e6770fe68fbe</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>hwloc_bitmap_t</type>
      <name>hwloc_bitmap_dup</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gae679434c1a5f41d3560a8a7e2c1b0dee</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_copy</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga036068a22d1aa5cb9ff1df348b525ff3</anchor>
      <arglist>(hwloc_bitmap_t dst, hwloc_const_bitmap_t src)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_snprintf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga9511644657030a021dce9941e3cda583</anchor>
      <arglist>(char *restrict buf, size_t buflen, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_asprintf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga0fece972134fdecf2da9bc7a11dd827e</anchor>
      <arglist>(char **strp, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_sscanf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga064a85c643a364b60acbfa0f3663fc18</anchor>
      <arglist>(hwloc_bitmap_t bitmap, const char *restrict string)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_list_snprintf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga3d06e6447edeb61e56a9ad83f31f0a37</anchor>
      <arglist>(char *restrict buf, size_t buflen, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_list_asprintf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga39c0f1b02053de66d205c9f8260b3665</anchor>
      <arglist>(char **strp, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_list_sscanf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaf9314311d482c621e17c6a53a9fe993e</anchor>
      <arglist>(hwloc_bitmap_t bitmap, const char *restrict string)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_taskset_snprintf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga5beb68a987fe48e5c5edef06ab260e14</anchor>
      <arglist>(char *restrict buf, size_t buflen, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_taskset_asprintf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga391312513f17c985d871f18367c59512</anchor>
      <arglist>(char **strp, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_taskset_sscanf</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga4f23db5041240e03e348752f8bd6ec2d</anchor>
      <arglist>(hwloc_bitmap_t bitmap, const char *restrict string)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_zero</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaa97c5217613c8cae9862287170ea2132</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_fill</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga07b2de8786b3cdb22f21d9dd42588275</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_only</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga037032ba6aaf49c2e6836e2dbb7855c4</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_allbut</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga3b944660dcfcb8d655883742b3ea86a7</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_from_ulong</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaba53aebd49962acea4e3e0ff160172e3</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_from_ith_ulong</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga6066e095e6f23302f0a1ca7284d0b651</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned i, unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_set</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga55480e96ab47941bfebb9aed676526a2</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_set_range</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga7a1d8530b282397f56b0d82b17e6d13d</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned begin, int end)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_set_ith_ulong</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gab16b6f6383b51f9c314abe15478ff6ff</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned i, unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_clr</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gabddf36d47882c0a4f1e2b3c76cac877c</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_clr_range</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga207ae23381d7b37fb97fd1f906c9c5ba</anchor>
      <arglist>(hwloc_bitmap_t bitmap, unsigned begin, int end)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_singlify</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga36d802130aa5d96bc1b5a90d2f0f3765</anchor>
      <arglist>(hwloc_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>hwloc_bitmap_to_ulong</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gab6a6bae22a33ec6adfc169a953c13e6c</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>hwloc_bitmap_to_ith_ulong</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gac92e6404f648d6b99d2c626b05b2ffa4</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap, unsigned i)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_isset</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga11340dd487f110bb84f0a6e4ae90bd06</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap, unsigned id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_iszero</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga5b64be28f5a7176ed8ad0d6a90bdf108</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_isfull</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga5fdcb8c19c336511c37076d649e74af4</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_first</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga3ec1ced43afadf8c0a7197320bc016ed</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_next</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaab7240b4b644693b775dad17b44ffd68</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap, int prev)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_last</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga3d9b08b060a59bbab5a4ea6d0a1ed7a6</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_weight</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga0200466842c0e1f5da75e84dde460b10</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_or</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga1281994d6cc3b5079224190ca820ad93</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_and</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga4a3e12ab9e20cfec66a3dc2db16806be</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_andnot</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga091e9039ce7b6fe927260cd1b08d25c4</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_xor</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaf462ead487d1891e37f4443a397d3c42</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_bitmap_not</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga8cd2c26d71510138b22a12db41419742</anchor>
      <arglist>(hwloc_bitmap_t res, hwloc_const_bitmap_t bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_intersects</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaefa070f9232857ba5a57297ea9a08ea2</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_isincluded</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga0526e03db81956fb02acc8260b66d6a4</anchor>
      <arglist>(hwloc_const_bitmap_t sub_bitmap, hwloc_const_bitmap_t super_bitmap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_isequal</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga32376b1405e57472b73c3aa53bb39663</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_compare_first</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>gaa39cda1510ab721b5c012c76ead72365</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_bitmap_compare</name>
      <anchorfile>a00095.html</anchorfile>
      <anchor>ga14cdbc0ef0a705e84999bd48a2e649f3</anchor>
      <arglist>(hwloc_const_bitmap_t bitmap1, hwloc_const_bitmap_t bitmap2)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_diff</name>
    <title>Topology differences</title>
    <filename>a00096.html</filename>
    <class kind="union">hwloc_topology_diff_obj_attr_u</class>
    <class kind="struct">hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_generic_s</class>
    <class kind="struct">hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_uint64_s</class>
    <class kind="struct">hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_string_s</class>
    <class kind="union">hwloc_topology_diff_u</class>
    <class kind="struct">hwloc_topology_diff_u::hwloc_topology_diff_generic_s</class>
    <class kind="struct">hwloc_topology_diff_u::hwloc_topology_diff_obj_attr_s</class>
    <class kind="struct">hwloc_topology_diff_u::hwloc_topology_diff_too_complex_s</class>
    <member kind="typedef">
      <type>enum hwloc_topology_diff_obj_attr_type_e</type>
      <name>hwloc_topology_diff_obj_attr_type_t</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga5f2dd099de2cacdc0d0d4858154b971a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum hwloc_topology_diff_type_e</type>
      <name>hwloc_topology_diff_type_t</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga4b86adb00c8b2d09ebc4ef8f3bfd92b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>union hwloc_topology_diff_u *</type>
      <name>hwloc_topology_diff_t</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga2cf1b17332fe5d95f2198f6340cfd288</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_topology_diff_obj_attr_type_e</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga86f044210b0a9e9fa83acbdbbf7e05fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_DIFF_OBJ_ATTR_SIZE</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gga86f044210b0a9e9fa83acbdbbf7e05fdabc7f0c7ed0b6864e902f4b70f2c7bc94</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_DIFF_OBJ_ATTR_NAME</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gga86f044210b0a9e9fa83acbdbbf7e05fda94a8f37c51d62d15ac6192665dd49310</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_DIFF_OBJ_ATTR_INFO</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gga86f044210b0a9e9fa83acbdbbf7e05fda63430bf932434bc456282f636d39c2c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_topology_diff_type_e</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga38b28b7423b85a3321e6d0062d5f83d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_DIFF_OBJ_ATTR</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gga38b28b7423b85a3321e6d0062d5f83d0accbcaee230f79989debb284c8626f0c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_DIFF_TOO_COMPLEX</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gga38b28b7423b85a3321e6d0062d5f83d0a3dc01fdeff355efe3fb2516bb454a147</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_topology_diff_apply_flags_e</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gada4c1273ce020afaf02b649496f7edf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_TOPOLOGY_DIFF_APPLY_REVERSE</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ggada4c1273ce020afaf02b649496f7edf5a821a160512d67ea0dd05dab873d2fc54</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_build</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga8a1754f69fbb31364ea3ea2c39827c9f</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_topology_t newtopology, unsigned long flags, hwloc_topology_diff_t *diff)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_apply</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gae7b350d7e7478a4c6047b08aa6544f40</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_topology_diff_t diff, unsigned long flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_destroy</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gab531db0e4ca3e590d48358c90f59c197</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_topology_diff_t diff)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_load_xml</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gae82a885a377aff78936c5f175b5980f8</anchor>
      <arglist>(hwloc_topology_t topology, const char *xmlpath, hwloc_topology_diff_t *diff, char **refname)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_export_xml</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>gacceb7e97b5689d0405db618bb5541012</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_topology_diff_t diff, const char *refname, const char *xmlpath)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_load_xmlbuffer</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga09afc24f1c9860465ec352c4ac518dd4</anchor>
      <arglist>(hwloc_topology_t topology, const char *xmlbuffer, int buflen, hwloc_topology_diff_t *diff, char **refname)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_topology_diff_export_xmlbuffer</name>
      <anchorfile>a00096.html</anchorfile>
      <anchor>ga88e72855746c81e2341580fa8d619a51</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_topology_diff_t diff, const char *refname, char **xmlbuffer, int *buflen)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_disc_components</name>
    <title>Components and Plugins: Discovery components</title>
    <filename>a00097.html</filename>
    <class kind="struct">hwloc_disc_component</class>
    <member kind="typedef">
      <type>enum hwloc_disc_component_type_e</type>
      <name>hwloc_disc_component_type_t</name>
      <anchorfile>a00097.html</anchorfile>
      <anchor>ga5c6112c1a7497e5f481b9634059d47b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_disc_component_type_e</name>
      <anchorfile>a00097.html</anchorfile>
      <anchor>ga0dceb95fada01e5e4558429912053ca7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_DISC_COMPONENT_TYPE_CPU</name>
      <anchorfile>a00097.html</anchorfile>
      <anchor>gga0dceb95fada01e5e4558429912053ca7aa09e6a39b61eda5364eada626da88ecc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_DISC_COMPONENT_TYPE_GLOBAL</name>
      <anchorfile>a00097.html</anchorfile>
      <anchor>gga0dceb95fada01e5e4558429912053ca7a7d7e7114cca4165eee72efb9b39b3d6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_DISC_COMPONENT_TYPE_MISC</name>
      <anchorfile>a00097.html</anchorfile>
      <anchor>gga0dceb95fada01e5e4558429912053ca7aaf4d61f8e2c7c28e88cd81bb565695b7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_disc_backends</name>
    <title>Components and Plugins: Discovery backends</title>
    <filename>a00098.html</filename>
    <class kind="struct">hwloc_backend</class>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_backend_flag_e</name>
      <anchorfile>a00098.html</anchorfile>
      <anchor>ga21513209613570877b6bfa9898106f2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_BACKEND_FLAG_NEED_LEVELS</name>
      <anchorfile>a00098.html</anchorfile>
      <anchor>gga21513209613570877b6bfa9898106f2aadc96f2cf3bdd5d41e102dfa1e1976b24</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>struct hwloc_backend *</type>
      <name>hwloc_backend_alloc</name>
      <anchorfile>a00098.html</anchorfile>
      <anchor>ga330a0b581de4817d0cf1e7401db22436</anchor>
      <arglist>(struct hwloc_disc_component *component)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_backend_enable</name>
      <anchorfile>a00098.html</anchorfile>
      <anchor>gaa4edf46c5d88eef53b3b8f572d88b9c5</anchor>
      <arglist>(struct hwloc_topology *topology, struct hwloc_backend *backend)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_backends_get_obj_cpuset</name>
      <anchorfile>a00098.html</anchorfile>
      <anchor>ga505a5470745bf0b601f4d25a69601411</anchor>
      <arglist>(struct hwloc_backend *caller, struct hwloc_obj *obj, hwloc_bitmap_t cpuset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_backends_notify_new_object</name>
      <anchorfile>a00098.html</anchorfile>
      <anchor>gad8ac8bba9ab6b9af423baba0c8337c6b</anchor>
      <arglist>(struct hwloc_backend *caller, struct hwloc_obj *obj)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_generic_components</name>
    <title>Components and Plugins: Generic components</title>
    <filename>a00099.html</filename>
    <class kind="struct">hwloc_component</class>
    <member kind="typedef">
      <type>enum hwloc_component_type_e</type>
      <name>hwloc_component_type_t</name>
      <anchorfile>a00099.html</anchorfile>
      <anchor>ga0aebfa65317af10bb18d7d35f6dc05b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>hwloc_component_type_e</name>
      <anchorfile>a00099.html</anchorfile>
      <anchor>ga397a1bf7d21dd073320ad0045340f463</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_COMPONENT_TYPE_DISC</name>
      <anchorfile>a00099.html</anchorfile>
      <anchor>gga397a1bf7d21dd073320ad0045340f463a5d6e561f467fe9795a29e7368b613900</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>HWLOC_COMPONENT_TYPE_XML</name>
      <anchorfile>a00099.html</anchorfile>
      <anchor>gga397a1bf7d21dd073320ad0045340f463adea00cd839c2141c032e1569fd5592bd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_components_core_funcs</name>
    <title>Components and Plugins: Core functions to be used by components</title>
    <filename>a00100.html</filename>
    <member kind="typedef">
      <type>void(*</type>
      <name>hwloc_report_error_t</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>gac2286a3c84a2417f72400bd65bccb99e</anchor>
      <arglist>)(const char *msg, int line)</arglist>
    </member>
    <member kind="function">
      <type>struct hwloc_obj *</type>
      <name>hwloc_insert_object_by_cpuset</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>gafd853fd67e12c32916201d8303ef39d2</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_obj_t obj)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_report_os_error</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>ga3ae154590328522a864f4a7fb453b562</anchor>
      <arglist>(const char *msg, int line)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_hide_errors</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>gab527c1e0f243f057d31a724794ad9f88</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>struct hwloc_obj *</type>
      <name>hwloc__insert_object_by_cpuset</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>ga56d4e5fec377bf09f136b4e8e280e30f</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_obj_t obj, hwloc_report_error_t report_error)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hwloc_insert_object_by_parent</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>ga0ef97debde503b56367689b07bb3e901</anchor>
      <arglist>(struct hwloc_topology *topology, hwloc_obj_t parent, hwloc_obj_t obj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static struct hwloc_obj *</type>
      <name>hwloc_alloc_setup_object</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>ga64d210fcfd7e517095319420bf40ed0a</anchor>
      <arglist>(hwloc_obj_type_t type, signed os_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_fill_object_sets</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>gabb240e5b856a34963078ccec9a785ac1</anchor>
      <arglist>(hwloc_obj_t obj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_plugin_check_namespace</name>
      <anchorfile>a00100.html</anchorfile>
      <anchor>gac2eaed287bb215cf0bd33014e9e1d374</anchor>
      <arglist>(const char *pluginname, const char *symbol)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_components_pci_funcs</name>
    <title>Components and Plugins: PCI functions to be used by components</title>
    <filename>a00101.html</filename>
    <member kind="function">
      <type>int</type>
      <name>hwloc_insert_pci_device_list</name>
      <anchorfile>a00101.html</anchorfile>
      <anchor>ga2ea3e9c02eabd8c5768745c8a99d7fed</anchor>
      <arglist>(struct hwloc_backend *backend, struct hwloc_obj *first_obj)</arglist>
    </member>
    <member kind="function">
      <type>unsigned</type>
      <name>hwloc_pci_find_cap</name>
      <anchorfile>a00101.html</anchorfile>
      <anchor>ga4bed8bd79337d4bc84b884a6ccd14f16</anchor>
      <arglist>(const unsigned char *config, unsigned cap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_pci_find_linkspeed</name>
      <anchorfile>a00101.html</anchorfile>
      <anchor>ga749e75eeea108be0608fcba7c113f736</anchor>
      <arglist>(const unsigned char *config, unsigned offset, float *linkspeed)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_pci_prepare_bridge</name>
      <anchorfile>a00101.html</anchorfile>
      <anchor>ga80fa1e3d882b5c2225dd6dd01e098ad0</anchor>
      <arglist>(hwloc_obj_t obj, const unsigned char *config)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux</name>
    <title>Linux-specific helpers</title>
    <filename>a00102.html</filename>
    <member kind="function">
      <type>int</type>
      <name>hwloc_linux_parse_cpumap_file</name>
      <anchorfile>a00102.html</anchorfile>
      <anchor>gaa5a608236c3c0757b0b79818f1d429fb</anchor>
      <arglist>(FILE *file, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_linux_set_tid_cpubind</name>
      <anchorfile>a00102.html</anchorfile>
      <anchor>ga630e699c56b5f3d2e534b475944c216e</anchor>
      <arglist>(hwloc_topology_t topology, pid_t tid, hwloc_const_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_linux_get_tid_cpubind</name>
      <anchorfile>a00102.html</anchorfile>
      <anchor>gaddcb985cb56337a4d6693cee72e7bf94</anchor>
      <arglist>(hwloc_topology_t topology, pid_t tid, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hwloc_linux_get_tid_last_cpu_location</name>
      <anchorfile>a00102.html</anchorfile>
      <anchor>gacc7945c2d06c6db1e58f8fd953009134</anchor>
      <arglist>(hwloc_topology_t topology, pid_t tid, hwloc_bitmap_t set)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux_libnuma_ulongs</name>
    <title>Interoperability with Linux libnuma unsigned long masks</title>
    <filename>a00103.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cpuset_to_linux_libnuma_ulongs</name>
      <anchorfile>a00103.html</anchorfile>
      <anchor>gae577962390f84b0b12f503314ab0f972</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset, unsigned long *mask, unsigned long *maxnode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_nodeset_to_linux_libnuma_ulongs</name>
      <anchorfile>a00103.html</anchorfile>
      <anchor>ga585ccf818d2f217f4e53f9c6506c3128</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, unsigned long *mask, unsigned long *maxnode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cpuset_from_linux_libnuma_ulongs</name>
      <anchorfile>a00103.html</anchorfile>
      <anchor>ga0e9d051e068940f722f6fdc61814149d</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const unsigned long *mask, unsigned long maxnode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_nodeset_from_linux_libnuma_ulongs</name>
      <anchorfile>a00103.html</anchorfile>
      <anchor>gad8491c672eb2594cc5e7b0a026867192</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, const unsigned long *mask, unsigned long maxnode)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_linux_libnuma_bitmask</name>
    <title>Interoperability with Linux libnuma bitmask</title>
    <filename>a00104.html</filename>
    <member kind="function" static="yes">
      <type>static struct bitmask *</type>
      <name>hwloc_cpuset_to_linux_libnuma_bitmask</name>
      <anchorfile>a00104.html</anchorfile>
      <anchor>gad8769b4544c0aeef154fc8223144e343</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t cpuset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static struct bitmask *</type>
      <name>hwloc_nodeset_to_linux_libnuma_bitmask</name>
      <anchorfile>a00104.html</anchorfile>
      <anchor>gadfaa14c1bec76965ac2e05f0c3f3473b</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cpuset_from_linux_libnuma_bitmask</name>
      <anchorfile>a00104.html</anchorfile>
      <anchor>ga37c8421110b1842414809019098794ef</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const struct bitmask *bitmask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_nodeset_from_linux_libnuma_bitmask</name>
      <anchorfile>a00104.html</anchorfile>
      <anchor>gaaca6bff3086aad5bdbd2f4c5f0ad14cc</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_nodeset_t nodeset, const struct bitmask *bitmask)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_glibc_sched</name>
    <title>Interoperability with glibc sched affinity</title>
    <filename>a00105.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cpuset_to_glibc_sched_affinity</name>
      <anchorfile>a00105.html</anchorfile>
      <anchor>ga88699a0c86c66a9b7baa3468cd9d24c3</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_const_cpuset_t hwlocset, cpu_set_t *schedset, size_t schedsetsize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cpuset_from_glibc_sched_affinity</name>
      <anchorfile>a00105.html</anchorfile>
      <anchor>ga8ec807ec17de57fa77aa5cfd6c8e4bfb</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_cpuset_t hwlocset, const cpu_set_t *schedset, size_t schedsetsize)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_opencl</name>
    <title>Interoperability with OpenCL</title>
    <filename>a00106.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_opencl_get_device_cpuset</name>
      <anchorfile>a00106.html</anchorfile>
      <anchor>gacc32737d4648d16b0d292438f210ec90</anchor>
      <arglist>(hwloc_topology_t topology, cl_device_id device, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_opencl_get_device_osdev_by_index</name>
      <anchorfile>a00106.html</anchorfile>
      <anchor>gae39352d124cb330eb37b84b418ed6cc5</anchor>
      <arglist>(hwloc_topology_t topology, unsigned platform_index, unsigned device_index)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_opencl_get_device_osdev</name>
      <anchorfile>a00106.html</anchorfile>
      <anchor>gadabfa6516aa12e5d8f79b9b4dd9f3cf8</anchor>
      <arglist>(hwloc_topology_t topology, cl_device_id device)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_cuda</name>
    <title>Interoperability with the CUDA Driver API</title>
    <filename>a00107.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cuda_get_device_pci_ids</name>
      <anchorfile>a00107.html</anchorfile>
      <anchor>ga1084285e8ff8b7df91c28917637481c6</anchor>
      <arglist>(hwloc_topology_t topology, CUdevice cudevice, int *domain, int *bus, int *dev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cuda_get_device_cpuset</name>
      <anchorfile>a00107.html</anchorfile>
      <anchor>gaec41c6b4dc3361156beb7dea2a74f5a3</anchor>
      <arglist>(hwloc_topology_t topology, CUdevice cudevice, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_cuda_get_device_pcidev</name>
      <anchorfile>a00107.html</anchorfile>
      <anchor>ga11fed607fa404e29e8da010f3ec128e4</anchor>
      <arglist>(hwloc_topology_t topology, CUdevice cudevice)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_cuda_get_device_osdev</name>
      <anchorfile>a00107.html</anchorfile>
      <anchor>ga252cb72175f1a2d682f883a0add80a66</anchor>
      <arglist>(hwloc_topology_t topology, CUdevice cudevice)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_cuda_get_device_osdev_by_index</name>
      <anchorfile>a00107.html</anchorfile>
      <anchor>ga12ee892994ed037e8f64bbffda02cf2e</anchor>
      <arglist>(hwloc_topology_t topology, unsigned idx)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_cudart</name>
    <title>Interoperability with the CUDA Runtime API</title>
    <filename>a00108.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cudart_get_device_pci_ids</name>
      <anchorfile>a00108.html</anchorfile>
      <anchor>gad8b701d9a34923e34bd58defd4c1e704</anchor>
      <arglist>(hwloc_topology_t topology, int idx, int *domain, int *bus, int *dev)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_cudart_get_device_cpuset</name>
      <anchorfile>a00108.html</anchorfile>
      <anchor>ga187ca00c6e12800a25151ce331620980</anchor>
      <arglist>(hwloc_topology_t topology, int idx, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_cudart_get_device_pcidev</name>
      <anchorfile>a00108.html</anchorfile>
      <anchor>gaeda4e6efbb36b518b2c286434ad23bb2</anchor>
      <arglist>(hwloc_topology_t topology, int idx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_cudart_get_device_osdev_by_index</name>
      <anchorfile>a00108.html</anchorfile>
      <anchor>gac0f3eeaf7712919f298097b1a21307b0</anchor>
      <arglist>(hwloc_topology_t topology, unsigned idx)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_nvml</name>
    <title>Interoperability with the NVIDIA Management Library</title>
    <filename>a00109.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_nvml_get_device_cpuset</name>
      <anchorfile>a00109.html</anchorfile>
      <anchor>ga26cf0036d09ec4d7cb692380fac9659d</anchor>
      <arglist>(hwloc_topology_t topology, nvmlDevice_t device, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_nvml_get_device_osdev_by_index</name>
      <anchorfile>a00109.html</anchorfile>
      <anchor>gacd50fd0e2766ee05bc13234b46714756</anchor>
      <arglist>(hwloc_topology_t topology, unsigned idx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_nvml_get_device_osdev</name>
      <anchorfile>a00109.html</anchorfile>
      <anchor>gaf176159b5760a191871eff23f5a69ee9</anchor>
      <arglist>(hwloc_topology_t topology, nvmlDevice_t device)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_gl</name>
    <title>Interoperability with OpenGL displays</title>
    <filename>a00110.html</filename>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_gl_get_display_osdev_by_port_device</name>
      <anchorfile>a00110.html</anchorfile>
      <anchor>ga4d66a77e4fe4051d6e5b772c1b8fcec5</anchor>
      <arglist>(hwloc_topology_t topology, unsigned port, unsigned device)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_gl_get_display_osdev_by_name</name>
      <anchorfile>a00110.html</anchorfile>
      <anchor>ga2d36ef441d1dfddf49cfb5c01bd58d8e</anchor>
      <arglist>(hwloc_topology_t topology, const char *name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_gl_get_display_by_osdev</name>
      <anchorfile>a00110.html</anchorfile>
      <anchor>ga936665ab5f0d29a081c4b78d11f4ad07</anchor>
      <arglist>(hwloc_topology_t topology, hwloc_obj_t osdev, unsigned *port, unsigned *device)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_intel_mic</name>
    <title>Interoperability with Intel Xeon Phi (MIC)</title>
    <filename>a00111.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_intel_mic_get_device_cpuset</name>
      <anchorfile>a00111.html</anchorfile>
      <anchor>ga91bafa51abe81527f72437e24ad5944e</anchor>
      <arglist>(hwloc_topology_t topology, int idx, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_intel_mic_get_device_osdev_by_index</name>
      <anchorfile>a00111.html</anchorfile>
      <anchor>gabce0465e853de74be84cafcb96dd48ab</anchor>
      <arglist>(hwloc_topology_t topology, unsigned idx)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_openfabrics</name>
    <title>Interoperability with OpenFabrics</title>
    <filename>a00112.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_ibv_get_device_cpuset</name>
      <anchorfile>a00112.html</anchorfile>
      <anchor>ga3ea0d838c1e7f1671b35687aae2da6ae</anchor>
      <arglist>(hwloc_topology_t topology, struct ibv_device *ibdev, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_ibv_get_device_osdev_by_name</name>
      <anchorfile>a00112.html</anchorfile>
      <anchor>ga31d794567f4420bbdc8baa7bf8bf6138</anchor>
      <arglist>(hwloc_topology_t topology, const char *ibname)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static hwloc_obj_t</type>
      <name>hwloc_ibv_get_device_osdev</name>
      <anchorfile>a00112.html</anchorfile>
      <anchor>ga7324cdfd5db6ed2669c051ef7e1b64e1</anchor>
      <arglist>(hwloc_topology_t topology, struct ibv_device *ibdev)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>hwlocality_myriexpress</name>
    <title>Interoperability with Myrinet Express</title>
    <filename>a00113.html</filename>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_mx_board_get_device_cpuset</name>
      <anchorfile>a00113.html</anchorfile>
      <anchor>ga868dfb8232333858e1e159c62ff9b682</anchor>
      <arglist>(hwloc_topology_t topology, unsigned id, hwloc_cpuset_t set)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hwloc_mx_endpoint_get_device_cpuset</name>
      <anchorfile>a00113.html</anchorfile>
      <anchor>ga1d01f76cdd4a408ad01f105deeef3bba</anchor>
      <arglist>(hwloc_topology_t topology, mx_endpoint_t endpoint, hwloc_cpuset_t set)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_backend</name>
    <filename>a00029.html</filename>
    <member kind="variable">
      <type>unsigned long</type>
      <name>flags</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>aaa8eafe495aadd81c2e3c4ec527c10ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>is_custom</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>a5209e6508c110dcd4c3c296466f07cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>is_thissystem</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ab8806f6db077f1bb2e9d63ec99223f7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>private_data</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>a2ea5bd36b7f06efdb65b98b32af16c68</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void(*</type>
      <name>disable</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>aea8210bbb2a885cbe2f3d213b00966a0</anchor>
      <arglist>)(struct hwloc_backend *backend)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>discover</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>a1adfa49eb1eb832b56d097df8d37a660</anchor>
      <arglist>)(struct hwloc_backend *backend)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>get_obj_cpuset</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>a5715ebb86f3290a1d8c86fbec8a7dae7</anchor>
      <arglist>)(struct hwloc_backend *backend, struct hwloc_backend *caller, struct hwloc_obj *obj, hwloc_bitmap_t cpuset)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>notify_new_object</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>afade8464c2b52d0cc63a705f556e81ed</anchor>
      <arglist>)(struct hwloc_backend *backend, struct hwloc_backend *caller, struct hwloc_obj *obj)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_component</name>
    <filename>a00032.html</filename>
    <member kind="variable">
      <type>unsigned</type>
      <name>abi</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a3250bd6fc9713946991d805e48091b2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_component_type_t</type>
      <name>type</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a789208ada7e17492cfd3a5a88a6bb0ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>flags</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>ab8043c5b4cc0e81aabba586ccb194335</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>data</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a4b8cffd1d943c29fdc102b841b8598d4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_disc_component</name>
    <filename>a00033.html</filename>
    <member kind="variable">
      <type>hwloc_disc_component_type_t</type>
      <name>type</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ad070350354cbd665803fc86fe48cdb3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>a4064c3b5d9213027e87caebef380a840</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>excludes</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>a34144f4d59bf46524e4875194652412b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_backend *(*</type>
      <name>instantiate</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ad724ca2e525851042a70cab48481d048</anchor>
      <arglist>)(struct hwloc_disc_component *component, const void *data1, const void *data2, const void *data3)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>priority</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ae86d283c272c5ae24073a235efbf6b59</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_distances_s</name>
    <filename>a00034.html</filename>
    <member kind="variable">
      <type>unsigned</type>
      <name>relative_depth</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>a6fe066eaf62ee448aa05bab8e7217ff7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>nbobjs</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>a4ca2af858cebbce7324ec49903d09474</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float *</type>
      <name>latency</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>a0f70f48d1bfb18e5e2008825da2967c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>latency_max</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>aab61bd1d1ae2e121bfe793c973ec829e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>latency_base</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>a204416418049a272bfb51602fc676342</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj</name>
    <filename>a00036.html</filename>
    <member kind="variable">
      <type>hwloc_obj_type_t</type>
      <name>type</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>acc4f0803f244867e68fe0036800be5de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>os_index</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a61a7a80a68eaccbaaa28269e678c81a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>abb709ec38f2970677e4e57d1d30be96d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_memory_s</type>
      <name>memory</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a1dc830816716213b5f797e4052487864</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_obj_attr_u *</type>
      <name>attr</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>accd40e29f71f19e88db62ea3df02adc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a9d82690370275d42d652eccdea5d3ee5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>logical_index</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a0d07fb7b8935e137c94d75a3eb492ae9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>signed</type>
      <name>os_level</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a68766f0b1c4d61b5bad87e3b81dacfde</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>next_cousin</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a85a788017457129589318b6c39451acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>prev_cousin</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ac715989f55ff5a0eb6be2969ee477ec0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>parent</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>adc494f6aed939992be1c55cca5822900</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>sibling_rank</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>aaa6043eee6f55869933c1d974efd9acd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>next_sibling</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a7f2343ed476fe4942e6fffd4cade1b40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>prev_sibling</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a7b89e8c189876c0158a9282aaaf17f50</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>arity</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>aac3f6da35c9b57599909a44ce2b716c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj **</type>
      <name>children</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a04d05403da37bfe17cd63b7c7dd07b1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>first_child</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>af51d08a0a79dba517c06c5afedc8d2dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj *</type>
      <name>last_child</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a84bd65634dbc55f4158b74443a9bd04f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>userdata</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a76fd3ac94401cf32dfccc3a3a8de68a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>cpuset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a67925e0f2c47f50408fbdb9bddd0790f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>complete_cpuset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a91788a9da687beb7224cc1fd7b75208c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>online_cpuset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a8842d56c2975380f327ea401c5f53564</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_cpuset_t</type>
      <name>allowed_cpuset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>afa3c59a6dd3da8ffa48710780a1bfb34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_nodeset_t</type>
      <name>nodeset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a08f0d0e16c619a6e653526cbee4ffea3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_nodeset_t</type>
      <name>complete_nodeset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ac38c4012127525ef74c5615c526f4c2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_nodeset_t</type>
      <name>allowed_nodeset</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a19e3d0a5951a7510fc4fc4722a9bf531</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_distances_s **</type>
      <name>distances</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a9a5dd75596edc48fe834f81988cdc0d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>distances_count</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a8be6f63eca4da91000c832280db927b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_info_s *</type>
      <name>infos</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ad59bcd73a10260a0e239be5c4b5888bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>infos_count</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a9843acc28cfbba903b63ea14b137ff70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>symmetric_subtree</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>a0f41a1d67dc6b661906f2217563637f3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>hwloc_obj_attr_u</name>
    <filename>a00037.html</filename>
    <class kind="struct">hwloc_obj_attr_u::hwloc_bridge_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_cache_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_group_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_osdev_attr_s</class>
    <class kind="struct">hwloc_obj_attr_u::hwloc_pcidev_attr_s</class>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_cache_attr_s</type>
      <name>cache</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ab5a8ae3bf490e6b1071fea53f7382836</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_group_attr_s</type>
      <name>group</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ae4ba157cc313e2cdd9a82f1c1df7aaa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_pcidev_attr_s</type>
      <name>pcidev</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>a4203d713ce0f5beaa6ee0e9bdac70828</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_bridge_attr_s</type>
      <name>bridge</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>adbdf280699dd84c7619cd8d9edc0d958</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_attr_u::hwloc_osdev_attr_s</type>
      <name>osdev</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>a22904c25fe44b323bab5c9bc52660fca</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_bridge_attr_s</name>
    <filename>a00030.html</filename>
    <member kind="variable">
      <type>union hwloc_obj_attr_u::hwloc_bridge_attr_s::@0</type>
      <name>upstream</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>a00ce9d99fc8792d1044fe25dc58605fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_obj_bridge_type_t</type>
      <name>upstream_type</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>a265dd2164aa2df4972e25a029da72125</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_obj_attr_u::hwloc_bridge_attr_s::@1</type>
      <name>downstream</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>acaf1ae02e37182bbb6966f8c4f35e499</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_obj_bridge_type_t</type>
      <name>downstream_type</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ac6a169b672d0e9f75756fd5665828b93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>a336c8b22893d5d734d8c9dfca4066b46</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_cache_attr_s</name>
    <filename>a00031.html</filename>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>size</name>
      <anchorfile>a00031.html</anchorfile>
      <anchor>abe5e788943ed04302976740c829674c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00031.html</anchorfile>
      <anchor>a5c8f7f39193736c2187ed626940835d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>linesize</name>
      <anchorfile>a00031.html</anchorfile>
      <anchor>a801e8a668e28caf06c8b88e9ae5c10db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>associativity</name>
      <anchorfile>a00031.html</anchorfile>
      <anchor>ad06525e474d1e2d1423ed71bb998592d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_obj_cache_type_t</type>
      <name>type</name>
      <anchorfile>a00031.html</anchorfile>
      <anchor>ad8edc8ded2b7a70d6abbc874801930f4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_group_attr_s</name>
    <filename>a00035.html</filename>
    <member kind="variable">
      <type>unsigned</type>
      <name>depth</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ad914eac61c77481e1b7037877bcc5579</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_osdev_attr_s</name>
    <filename>a00041.html</filename>
    <member kind="variable">
      <type>hwloc_obj_osdev_type_t</type>
      <name>type</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>a31e019e27e54ac6138d04be639bb96f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_attr_u::hwloc_pcidev_attr_s</name>
    <filename>a00042.html</filename>
    <member kind="variable">
      <type>unsigned short</type>
      <name>domain</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a8fba44988deb98613c1505a4019a34dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>bus</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>aae99e035e8d1387d7b8768aaa8eceb0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>dev</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a3d70c84a12f7e93d14c8d47bf4fd9dc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>func</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a695f32df53f4ef728670bfcf31b74e0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>class_id</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>aee735352d9f1686fd290fc6d6397c334</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>vendor_id</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>aad970ad19b62eb2d5df30e4802da4f4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>device_id</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a35b66064ab7d768caf7154e410caf0fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>subvendor_id</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a499db3d8cc89eaba04fcaef3df1cba97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned short</type>
      <name>subdevice_id</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>acac741aecd7a6db64f33409427e3971f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>revision</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a13ad54c93d08d8ac808e4de9674c5ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>linkspeed</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>a59b2fce35f7cbde86c4fd305d0ccda5f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_info_s</name>
    <filename>a00038.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>a115dde58f40338a2d3276d59c099857d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>a522efd7f5262a4493a73fa3476a041ce</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_memory_s</name>
    <filename>a00040.html</filename>
    <class kind="struct">hwloc_obj_memory_s::hwloc_obj_memory_page_type_s</class>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>total_memory</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>a8befd0b3f4f8b695cafed04b31f36a44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>local_memory</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>aace044c7f3fd9dffe50202296bf1dc5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>page_types_len</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>a208c27f4491077d7fb9ba5db8b29cb57</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_obj_memory_s::hwloc_obj_memory_page_type_s *</type>
      <name>page_types</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>a865eba7b12b986d72dbe7a2cfd97c50d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_obj_memory_s::hwloc_obj_memory_page_type_s</name>
    <filename>a00039.html</filename>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>size</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>a3f1d261766352288791512974665ce01</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>count</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>aa5b9fa36f1dc47ff3c345fe4dd06b260</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_cpubind_support</name>
    <filename>a00043.html</filename>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisproc_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a9403d51657a4d546b3ea9553a2973a27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisproc_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a77a09ddd78ee3e9ff5f532a6ac74f7eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_proc_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>aa166223d1c2a6de7256ab2d8b675a87e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_proc_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>aae705bc447adc163ead377362c4dfe9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisthread_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a57a89a4b5f1f74fa6cfe176f1e8b0798</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisthread_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a80d762e532d677dff262d83cc7bb1c60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thread_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a46fba33e307909ce256624687799dd6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thread_cpubind</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a8dd4d8531ed2eebdce1507e7d104154e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisproc_last_cpu_location</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ad2d5c94b738d92b7d2ede87e4e96321d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_proc_last_cpu_location</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a2554620148b7992a5093f338f5ae254f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisthread_last_cpu_location</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>a6be1f042fdce6bf41b4ea39f6f193808</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>hwloc_topology_diff_obj_attr_u</name>
    <filename>a00048.html</filename>
    <class kind="struct">hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_generic_s</class>
    <class kind="struct">hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_string_s</class>
    <class kind="struct">hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_uint64_s</class>
    <member kind="variable">
      <type>struct hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_generic_s</type>
      <name>generic</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>a68039c178e9961023345e0728382773b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_uint64_s</type>
      <name>uint64</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>abb15bfa7039ca8c97cd7ddfbdf779efa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_string_s</type>
      <name>string</name>
      <anchorfile>a00048.html</anchorfile>
      <anchor>a518a48e13168c31957589ce78820163e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_generic_s</name>
    <filename>a00045.html</filename>
    <member kind="variable">
      <type>hwloc_topology_diff_obj_attr_type_t</type>
      <name>type</name>
      <anchorfile>a00045.html</anchorfile>
      <anchor>a43f8133b7c30129b002c2d295e6580fc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_string_s</name>
    <filename>a00047.html</filename>
    <member kind="variable">
      <type>hwloc_topology_diff_obj_attr_type_t</type>
      <name>type</name>
      <anchorfile>a00047.html</anchorfile>
      <anchor>a784ab5827c1bf531b86d1f02f199d04d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>a00047.html</anchorfile>
      <anchor>a7ef48c66ccddf34d90edb4e4314b41c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>oldvalue</name>
      <anchorfile>a00047.html</anchorfile>
      <anchor>a33cc281d4417129064295a6ae7b79818</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>newvalue</name>
      <anchorfile>a00047.html</anchorfile>
      <anchor>a75f37f4add21cece50fee2701c3b77db</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_diff_obj_attr_u::hwloc_topology_diff_obj_attr_uint64_s</name>
    <filename>a00049.html</filename>
    <member kind="variable">
      <type>hwloc_topology_diff_obj_attr_type_t</type>
      <name>type</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>afe262b9aa558fe7bfecbd37a7a129197</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>index</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>adb772c3f1761ecadf496c80ae14afce3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>oldvalue</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>a44c0894dce563335d347fde6595c49c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hwloc_uint64_t</type>
      <name>newvalue</name>
      <anchorfile>a00049.html</anchorfile>
      <anchor>ae63aa59ad65aa783b730a4f213304eb4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>hwloc_topology_diff_u</name>
    <filename>a00051.html</filename>
    <class kind="struct">hwloc_topology_diff_u::hwloc_topology_diff_generic_s</class>
    <class kind="struct">hwloc_topology_diff_u::hwloc_topology_diff_obj_attr_s</class>
    <class kind="struct">hwloc_topology_diff_u::hwloc_topology_diff_too_complex_s</class>
    <member kind="variable">
      <type>struct hwloc_topology_diff_u::hwloc_topology_diff_generic_s</type>
      <name>generic</name>
      <anchorfile>a00051.html</anchorfile>
      <anchor>af1882d9c9d37c735a0e9b04069fab01f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_diff_u::hwloc_topology_diff_obj_attr_s</type>
      <name>obj_attr</name>
      <anchorfile>a00051.html</anchorfile>
      <anchor>a7245713f0f49e9f08fda28401a4f98fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_diff_u::hwloc_topology_diff_too_complex_s</type>
      <name>too_complex</name>
      <anchorfile>a00051.html</anchorfile>
      <anchor>adaaabee4029a455119ec3695c19632d6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_diff_u::hwloc_topology_diff_generic_s</name>
    <filename>a00044.html</filename>
    <member kind="variable">
      <type>hwloc_topology_diff_type_t</type>
      <name>type</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>a373005489fcddeba6319163e60263ae8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_topology_diff_u *</type>
      <name>next</name>
      <anchorfile>a00044.html</anchorfile>
      <anchor>a720aa94eb73cddfd8d6bf9fcbbfc035e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_diff_u::hwloc_topology_diff_obj_attr_s</name>
    <filename>a00046.html</filename>
    <member kind="variable">
      <type>hwloc_topology_diff_type_t</type>
      <name>type</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>a70a118ad288bd98f18c8ce6dc52e2d10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_topology_diff_u *</type>
      <name>next</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>acb9170a8eb3392656d70f7f3ef3e3eed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>obj_depth</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>aa28a121d9e158b1e610fb844ae1c8824</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>obj_index</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>adb9f4d414470b6670cde8d90759a6c1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_topology_diff_obj_attr_u</type>
      <name>diff</name>
      <anchorfile>a00046.html</anchorfile>
      <anchor>a91f0645f9062e892652357cd141c8ebb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_diff_u::hwloc_topology_diff_too_complex_s</name>
    <filename>a00050.html</filename>
    <member kind="variable">
      <type>hwloc_topology_diff_type_t</type>
      <name>type</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>a92084295a4d6faf17a95a7eea526ddbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union hwloc_topology_diff_u *</type>
      <name>next</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>a375afbaa043b109be689a7d9a3c7d153</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>obj_depth</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>a77e9eac7db8b073c9bf996f19eda1631</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>obj_index</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>a6dc6b3c4adb10db74d993e37afaa0580</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_discovery_support</name>
    <filename>a00052.html</filename>
    <member kind="variable">
      <type>unsigned char</type>
      <name>pu</name>
      <anchorfile>a00052.html</anchorfile>
      <anchor>ad7bb4ecf7a82f5a04fc632e9592ad3ab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_membind_support</name>
    <filename>a00053.html</filename>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisproc_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a36b3e388df9c6a249427cab7e3724749</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisproc_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a3fd51e6fa5f0dd800322301b46b08559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_proc_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a756f44912894b176bf979a1b65f12aac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_proc_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a9880cd2d605e316fc020167c49ca69ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_thisthread_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a0697af2e41f2e82b8ce71e3cc13f7eac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_thisthread_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a63b0b2e26157b472f5717ee93cc7c535</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>set_area_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a476c06f96b65c08b287cf2369966123b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>get_area_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a0a84e24a06f2fa487fe8c9605c6f68b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>alloc_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ae551abb27d2aa9ce008583488b845b98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>firsttouch_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a221098c339dbfab27bd2c9f5d32f123b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>bind_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ae7cdb1f1b5f0242a69f85b5a5538c764</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>interleave_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>a3c44c6012860bbeba8a0f4c19710858d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>replicate_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>afe608fb85eb2aa9706221ccb5fc4d415</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>nexttouch_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>ab0921af6e0cd6975812a80b8e5c7435c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>migrate_membind</name>
      <anchorfile>a00053.html</anchorfile>
      <anchor>aafa7683871a6a760246f9b35209caec5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>hwloc_topology_support</name>
    <filename>a00054.html</filename>
    <member kind="variable">
      <type>struct hwloc_topology_discovery_support *</type>
      <name>discovery</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>aea3fbd7653d987d81f848636c420504d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_cpubind_support *</type>
      <name>cpubind</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>adef2bb91f74c3e70a2a071393caf5f56</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct hwloc_topology_membind_support *</type>
      <name>membind</name>
      <anchorfile>a00054.html</anchorfile>
      <anchor>ac6eb62ae8bc0a68dce679a7107a36194</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>hwloc</name>
    <path>/home/bgoglin/SOFT/hwloc/tmp/hwloc/include/hwloc/</path>
    <filename>dir_5cb306d949c7931a3b6c77517393dd34.html</filename>
    <file>bitmap.h</file>
    <file>cuda.h</file>
    <file>cudart.h</file>
    <file>diff.h</file>
    <file>gl.h</file>
    <file>glibc-sched.h</file>
    <file>helper.h</file>
    <file>intel-mic.h</file>
    <file>linux-libnuma.h</file>
    <file>linux.h</file>
    <file>myriexpress.h</file>
    <file>nvml.h</file>
    <file>opencl.h</file>
    <file>openfabrics-verbs.h</file>
    <file>plugins.h</file>
  </compound>
  <compound kind="dir">
    <name>include</name>
    <path>/home/bgoglin/SOFT/hwloc/tmp/hwloc/include/</path>
    <filename>dir_d44c64559bbebec7f509842c48db8b23.html</filename>
    <dir>hwloc</dir>
    <file>hwloc.h</file>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Hardware Locality</title>
    <filename>index</filename>
    <docanchor file="index">Introduction</docanchor>
    <docanchor file="index" title="Installation">installation</docanchor>
    <docanchor file="index" title="CLI Examples">cli_examples</docanchor>
    <docanchor file="index" title="Programming Interface">interface</docanchor>
    <docanchor file="index" title="Portability">portability</docanchor>
    <docanchor file="index" title="API Example">interface_example</docanchor>
    <docanchor file="index" title="Questions and Bugs">bugs</docanchor>
    <docanchor file="index" title="History / Credits">history</docanchor>
    <docanchor file="index" title="Further Reading">further_read</docanchor>
  </compound>
</tagfile>
