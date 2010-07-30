#ifndef axum_formH
#define axum_formH

struct unique_id {
  unsigned short man_id;
  unsigned short prod_id;
  unsigned short id;
};

struct form_node_info {
  char name[32];
  unsigned short id;
  unique_id parent;
};

#endif
