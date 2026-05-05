typedef enum rt_plugin_value_type_e {
  RT_PLUGIN_VALUE_F64,
  RT_PLUGIN_VALUE_U64,
  RT_PLUGIN_VALUE_I64,
  RT_PLUGIN_VALUE_BOOL,
  RT_PLUGIN_VALUE_STRING,
} rt_plugin_value_type_t;

typedef struct rt_plugin_port_desc_e {
  const char *name;
  rt_plugin_value_type_t type;
} rt_plugin_port_desc_t;
