#ifndef axum_formH
#define axum_formH

#define WM_SET_ACTUATOR_DATA   WM_USER+0
struct set_actuator_data {
  unsigned short object;
  union mbn_data data;
};

#define WM_CONFIG_INFO         WM_USER+1
struct config_info {
  unsigned int obj;
  char Label[16];
  char Desc[64];
  unsigned int func_type, func_seq, func_func;
};

#define WM_RESIZE_FONTS        WM_USER+2
//No data

#define WM_ONLINE_STATUS       WM_USER+3

#define WM_MAMBANET_ERROR      WM_USER+4




struct unique_id {
  unsigned short man_id;
  unsigned short prod_id;
  unsigned short id;
};

struct configuration_info
{
  char Label[16];
  char Description[64];
};

struct form_node_info {
  char name[32];
  unsigned short id;
  unique_id parent;
  configuration_info *config_info;
};


/* recursively allocates memory and copies data type unions/structs */
void copy_datatype(unsigned char type, int size, const union mbn_data *src, union mbn_data *dest) {
  if(type == MBN_DATATYPE_OCTETS) {
    dest->Octets = (unsigned char *)malloc(size);
    memcpy((void *)dest->Octets, (void *)src->Octets, size);
  } else if(type == MBN_DATATYPE_ERROR) {
    dest->Error = (char *)malloc(size);
    memcpy((void *)dest->Error, (void *)src->Error, size);
  } else if(type == MBN_DATATYPE_OBJINFO) {
    dest->Info = (mbn_object *)malloc(sizeof(struct mbn_object));
    memcpy((void *)dest->Info, (void *)src->Info, sizeof(struct mbn_object));
    if(src->Info->SensorSize > 0) {
      copy_datatype(src->Info->SensorType, src->Info->SensorSize, &(src->Info->SensorMin), &(dest->Info->SensorMin));
      copy_datatype(src->Info->SensorType, src->Info->SensorSize, &(src->Info->SensorMax), &(dest->Info->SensorMax));
    }
    if(src->Info->ActuatorSize > 0) {
      copy_datatype(src->Info->ActuatorType, src->Info->ActuatorSize, &(src->Info->ActuatorMin), &(dest->Info->ActuatorMin));
      copy_datatype(src->Info->ActuatorType, src->Info->ActuatorSize, &(src->Info->ActuatorMax), &(dest->Info->ActuatorMax));
      copy_datatype(src->Info->ActuatorType, src->Info->ActuatorSize, &(src->Info->ActuatorDefault), &(dest->Info->ActuatorDefault));
    }
  } else {
    *dest = *src;
  }
}

/* recursively free()'s data type unions/structs allocated by parse_datatype() */
void free_datatype(unsigned char type, union mbn_data *data) {
  if(type == MBN_DATATYPE_ERROR)
    free(data->Error);
  if(type == MBN_DATATYPE_OCTETS)
    free(data->Octets);
  if(type == MBN_DATATYPE_OBJINFO) {
    if(data->Info->SensorSize > 0 && data->Info->SensorType != MBN_DATATYPE_OCTETS) {
      free_datatype(data->Info->SensorType, &(data->Info->SensorMin));
      free_datatype(data->Info->SensorType, &(data->Info->SensorMax));
    }
    if(data->Info->ActuatorSize > 0 && data->Info->ActuatorType != MBN_DATATYPE_OCTETS) {
      free_datatype(data->Info->ActuatorType, &(data->Info->ActuatorMin));
      free_datatype(data->Info->ActuatorType, &(data->Info->ActuatorMax));
      free_datatype(data->Info->ActuatorType, &(data->Info->ActuatorDefault));
    }
    free(data->Info);
  }
}

void trim(char * s) {
  char * p = s;
  int l = strlen(p);

  if (l>0)
  {
    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
  }
}

void space_to_newline(char *s) {
  int l = strlen(s);

  while(l) {
    if (isspace(s[l])) {
      s[l] = '\n';
    }
    l--;
  }
}




#endif
