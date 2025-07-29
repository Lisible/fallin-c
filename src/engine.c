#include "console.h"
#include "engine.h"
#include "jbb.h"
#include "panic.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAL_ID_MAX_LENGTH 64
#define FAL_NAME_MAX_LENGTH 64
#define FAL_DESCRIPTION_MAX_LENGTH 64
#define FAL_ILLUSTRATION_WIDTH 80
#define FAL_ILLUSTRATION_HEIGHT 10

#define FAL_ACTION_MAX_LENGTH 1024
#define FAL_TRIGGER_ACTION_MAX_COUNT 32
struct fal_trigger {
  char id[FAL_ID_MAX_LENGTH];
  char actions[FAL_ACTION_MAX_LENGTH][FAL_TRIGGER_ACTION_MAX_COUNT];
  int action_count;
};

struct fal_mob {
  char id[FAL_ID_MAX_LENGTH];
  char name[FAL_NAME_MAX_LENGTH];
  char description[FAL_DESCRIPTION_MAX_LENGTH];
  int hp;
  int atk;
  int def;
  int spawn_count;
  char illustration[FAL_ILLUSTRATION_WIDTH][FAL_ILLUSTRATION_HEIGHT];
};

#define FAL_NPC_SOLD_ITEM_TYPES_MAX_COUNT 32
#define FAL_NPC_BOUGHT_ITEM_TYPES_MAX_COUNT 32
struct fal_npc {
  char id[FAL_ID_MAX_LENGTH];
  char name[FAL_NAME_MAX_LENGTH];
  char greetings[FAL_DESCRIPTION_MAX_LENGTH];
  int sell_coefficient;
  char sold_item_types[FAL_ID_MAX_LENGTH][FAL_NPC_SOLD_ITEM_TYPES_MAX_COUNT];
  int sold_item_type_count;
  char bought_item_types[FAL_ID_MAX_LENGTH]
                        [FAL_NPC_BOUGHT_ITEM_TYPES_MAX_COUNT];
  int bought_item_type_count;
};

#define FAL_LOCATION_ACTION_MAX_COUNT 16
struct fal_location {
  char id[FAL_ID_MAX_LENGTH];
  char name[FAL_NAME_MAX_LENGTH];
  char description_day[FAL_DESCRIPTION_MAX_LENGTH];
  char description_night[FAL_DESCRIPTION_MAX_LENGTH];
  char illustration[FAL_ILLUSTRATION_WIDTH][FAL_ILLUSTRATION_HEIGHT];
  char actions[FAL_ACTION_MAX_LENGTH][FAL_LOCATION_ACTION_MAX_COUNT];
  int action_count;
  char origin_map[FAL_ID_MAX_LENGTH];
  char origin_actions[FAL_ACTION_MAX_LENGTH][FAL_LOCATION_ACTION_MAX_COUNT];
  int origin_action_count;
};

#define FAL_ITEM_CRAFT_TYPE_MAX_LENGTH 64
#define FAL_ITEM_CRAFT_TYPE_MAX_COUNT 16
#define FAL_ITEM_CRAFT_RECIPE_MAX_LENGTH 16
#define FAL_ITEM_CATEGORY_MAX_LENGTH 64
#define FAL_ITEM_ACTIONS_MAX_COUNT 32
#define FAL_ITEM_CHILD_MAX_LENGTH 64
struct fal_id_int {
  char id[FAL_ID_MAX_LENGTH];
  int value;
};
struct fal_item {
  char id[FAL_ID_MAX_LENGTH];
  char name[FAL_NAME_MAX_LENGTH];
  int price;
  int atk;
  int def;
  char craft_types[FAL_ITEM_CRAFT_TYPE_MAX_LENGTH]
                  [FAL_ITEM_CRAFT_TYPE_MAX_COUNT];
  int craft_type_count;
  char description[FAL_DESCRIPTION_MAX_LENGTH];
  struct fal_id_int craft_recipe[FAL_ITEM_CRAFT_RECIPE_MAX_LENGTH];
  int craft_recipe_length;
  int craft_quantity;
  char category[FAL_ITEM_CATEGORY_MAX_LENGTH];
  int loot_modifier;
  char child[FAL_ITEM_CHILD_MAX_LENGTH];
  bool stackable;
  bool useable;
  char actions[FAL_ACTION_MAX_LENGTH][FAL_ITEM_ACTIONS_MAX_COUNT];
  int action_count;
};

#define FAL_ENGINE_MAX_ITEM_COUNT 128
#define FAL_ENGINE_MAX_TRIGGER_COUNT 128
#define FAL_ENGINE_MAX_MOB_COUNT 128
#define FAL_ENGINE_MAX_NPC_COUNT 128
#define FAL_ENGINE_MAX_LOCATION_COUNT 128
struct fal_engine {
  struct fal_item items[FAL_ENGINE_MAX_ITEM_COUNT];
  struct fal_mob mobs[FAL_ENGINE_MAX_MOB_COUNT];
  struct fal_npc npcs[FAL_ENGINE_MAX_NPC_COUNT];
  struct fal_trigger triggers[FAL_ENGINE_MAX_TRIGGER_COUNT];
  struct fal_location locations[FAL_ENGINE_MAX_LOCATION_COUNT];
  fal_console *console;
  int item_count;
  int trigger_count;
  int mob_count;
  int npc_count;
  int location_count;
};

#define LOAD_ITEMS_PATH_BUF_CAPACITY 4096
#define LOAD_ITEMS_LINE_BUF_CAPACITY 1024

static bool parse_item(FILE *file, char *line_buf, int line_buf_cap,
                       struct fal_item *item) {
  while (strcmp(line_buf, ":endItem") != 0) {
    fal_jbb_read_line(file, line_buf, line_buf_cap);
    if (strcmp(line_buf, ":id") == 0) {
      fal_jbb_parse_string(file, item->id, FAL_ID_MAX_LENGTH);
    } else if (strcmp(line_buf, ":name") == 0) {
      fal_jbb_parse_string(file, item->name, FAL_NAME_MAX_LENGTH);
    } else if (strcmp(line_buf, ":price") == 0) {
      fal_jbb_parse_int(file, &item->price);
    } else if (strcmp(line_buf, ":atk") == 0) {
      fal_jbb_parse_int(file, &item->atk);
    } else if (strcmp(line_buf, ":def") == 0) {
      fal_jbb_parse_int(file, &item->def);
    } else if (strcmp(line_buf, ":description") == 0) {
      fal_jbb_parse_string(file, item->description, FAL_DESCRIPTION_MAX_LENGTH);
    } else if (strcmp(line_buf, ":craftRecipe") == 0) {
      fal_jbb_read_line(file, line_buf, line_buf_cap);

      item->craft_recipe_length = 0;
      char *token = strtok(line_buf, ";");
      while (token != NULL) {
        if (item->craft_recipe_length > FAL_ITEM_CRAFT_RECIPE_MAX_LENGTH) {
          return false;
        }
        char *key = strtok(token, ":");
        char *val = strtok(NULL, ":");
        if (key != NULL && val != NULL) {
          memcpy(item->craft_recipe[item->craft_recipe_length].id, key,
                 FAL_ID_MAX_LENGTH);
          item->craft_recipe[item->craft_recipe_length].value =
              strtol(val, NULL, 10);
          item->craft_recipe_length++;
        }

        token = strtok(NULL, ";");
      }
    } else if (strcmp(line_buf, ":craftQuantity") == 0) {
      fal_jbb_parse_int(file, &item->craft_quantity);
    } else if (strcmp(line_buf, ":category") == 0) {
      fal_jbb_parse_string(file, item->category, FAL_ITEM_CATEGORY_MAX_LENGTH);
    } else if (strcmp(line_buf, ":lootModifier") == 0) {
      fal_jbb_parse_int(file, &item->loot_modifier);
    } else if (strcmp(line_buf, ":child") == 0) {
      fal_jbb_parse_string(file, item->child, FAL_ITEM_CHILD_MAX_LENGTH);
    } else if (strcmp(line_buf, ":stackable") == 0) {
      fal_jbb_parse_bool(file, &item->stackable);
    } else if (strcmp(line_buf, ":useable") == 0) {
      fal_jbb_parse_bool(file, &item->stackable);
    } else if (strcmp(line_buf, ":actions") == 0) {
      item->action_count = 0;
      while (strcmp(line_buf, ":endActions") != 0) {
        fal_jbb_read_line(file, line_buf, line_buf_cap);

        if (item->action_count >= FAL_ITEM_ACTIONS_MAX_COUNT) {
          goto err;
        }

        if (strlen(line_buf) > FAL_ACTION_MAX_LENGTH) {
          goto err;
        }

        memcpy(item->actions[item->action_count++], line_buf,
               FAL_ACTION_MAX_LENGTH);
      }
    } else if (strcmp(line_buf, ":craftType") == 0) {
      fal_jbb_read_line(file, line_buf, line_buf_cap);
      item->craft_type_count = 0;
      char *token = strtok(line_buf, ";");
      while (token) {
        if (item->craft_type_count >= FAL_ITEM_CRAFT_TYPE_MAX_COUNT) {
          goto err;
        }
        memcpy(item->craft_types[item->craft_type_count++], line_buf,
               FAL_ITEM_CRAFT_TYPE_MAX_LENGTH);
        token = strtok(NULL, ";");
      }
    }
  }
  return true;
err:
  return false;
}

static bool parse_trigger(FILE *file, char *line_buf, int line_buf_cap,
                          struct fal_trigger *trigger) {
  while (strcmp(line_buf, ":endTrigger") != 0) {
    fal_jbb_read_line(file, line_buf, line_buf_cap);
    if (strcmp(line_buf, ":id") == 0) {
      fal_jbb_parse_string(file, trigger->id, FAL_ID_MAX_LENGTH);
    } else if (strcmp(line_buf, ":actions") == 0) {
      trigger->action_count = 0;
      while (strcmp(line_buf, ":endActions") != 0) {
        fal_jbb_read_line(file, line_buf, line_buf_cap);

        if (trigger->action_count >= FAL_TRIGGER_ACTION_MAX_COUNT) {
          goto err;
        }

        if (strlen(line_buf) > FAL_ACTION_MAX_LENGTH) {
          goto err;
        }

        memcpy(trigger->actions[trigger->action_count++], line_buf,
               FAL_ACTION_MAX_LENGTH);
      }
    }
  }
  return true;
err:
  return false;
}
static bool parse_mob(FILE *file, char *line_buf, int line_buf_cap,
                      struct fal_mob *mob) {
  while (strcmp(line_buf, ":endMob") != 0) {
    fal_jbb_read_line(file, line_buf, line_buf_cap);
    if (strcmp(line_buf, ":id") == 0) {
      fal_jbb_parse_string(file, mob->id, FAL_ID_MAX_LENGTH);
    } else if (strcmp(line_buf, ":name") == 0) {
      fal_jbb_parse_string(file, mob->name, FAL_NAME_MAX_LENGTH);
    } else if (strcmp(line_buf, ":desc") == 0) {
      fal_jbb_parse_string(file, mob->description, FAL_NAME_MAX_LENGTH);
    } else if (strcmp(line_buf, ":hp") == 0) {
      fal_jbb_parse_int(file, &mob->hp);
    } else if (strcmp(line_buf, ":atk") == 0) {
      fal_jbb_parse_int(file, &mob->hp);
    } else if (strcmp(line_buf, ":def") == 0) {
      fal_jbb_parse_int(file, &mob->hp);
    } else if (strcmp(line_buf, ":illustration") == 0) {
      for (int i = 0; i < FAL_ILLUSTRATION_HEIGHT; i++) {
        fal_jbb_parse_string(file, mob->illustration[i],
                             FAL_ILLUSTRATION_WIDTH);
      }
    }
  }
  return true;
err:
  return false;
}
static bool parse_npc(FILE *file, char *line_buf, int line_buf_cap,
                      struct fal_npc *npc) {
  while (strcmp(line_buf, ":endNpc") != 0) {
    fal_jbb_read_line(file, line_buf, line_buf_cap);
    if (strcmp(line_buf, ":id") == 0) {
      fal_jbb_parse_string(file, npc->id, FAL_ID_MAX_LENGTH);
    } else if (strcmp(line_buf, ":name") == 0) {
      fal_jbb_parse_string(file, npc->name, FAL_NAME_MAX_LENGTH);
    } else if (strcmp(line_buf, ":greetings") == 0) {
      fal_jbb_parse_string(file, npc->greetings, FAL_NAME_MAX_LENGTH);
    } else if (strcmp(line_buf, ":sellCoeff") == 0) {
      fal_jbb_parse_int(file, &npc->sell_coefficient);
    } else if (strcmp(line_buf, ":itemTypeSell") == 0) {
      fal_jbb_read_line(file, line_buf, line_buf_cap);
      npc->sold_item_type_count = 0;
      char *token = strtok(line_buf, ";");
      while (token) {
        if (npc->sold_item_type_count >= FAL_NPC_SOLD_ITEM_TYPES_MAX_COUNT) {
          goto err;
        }
        memcpy(npc->sold_item_types[npc->sold_item_type_count++], line_buf,
               FAL_ID_MAX_LENGTH);
        token = strtok(NULL, ";");
      }
    } else if (strcmp(line_buf, ":itemTypeBuy") == 0) {
      fal_jbb_read_line(file, line_buf, line_buf_cap);
      npc->sold_item_type_count = 0;
      char *token = strtok(line_buf, ";");
      while (token) {
        if (npc->sold_item_type_count >= FAL_NPC_SOLD_ITEM_TYPES_MAX_COUNT) {
          goto err;
        }
        memcpy(npc->sold_item_types[npc->sold_item_type_count++], line_buf,
               FAL_ID_MAX_LENGTH);
        token = strtok(NULL, ";");
      }
    }
  }
  return true;
err:
  return false;
}

static bool parse_location(FILE *file, char *line_buf, int line_buf_cap,
                           struct fal_location *location) {
  while (strcmp(line_buf, ":endLocation") != 0) {
    fal_jbb_read_line(file, line_buf, line_buf_cap);
    if (strcmp(line_buf, ":id") == 0) {
      fal_jbb_parse_string(file, location->id, FAL_ID_MAX_LENGTH);
    } else if (strcmp(line_buf, ":name") == 0) {
      fal_jbb_parse_string(file, location->name, FAL_NAME_MAX_LENGTH);
    } else if (strcmp(line_buf, ":descDay") == 0) {
      fal_jbb_parse_string(file, location->description_day,
                           FAL_DESCRIPTION_MAX_LENGTH);
    } else if (strcmp(line_buf, ":descNight") == 0) {
      fal_jbb_parse_string(file, location->description_night,
                           FAL_DESCRIPTION_MAX_LENGTH);
    } else if (strcmp(line_buf, ":descNight") == 0) {
      fal_jbb_parse_string(file, location->description_night,
                           FAL_DESCRIPTION_MAX_LENGTH);
    } else if (strcmp(line_buf, ":originMap") == 0) {
      fal_jbb_parse_string(file, location->origin_map,
                           FAL_DESCRIPTION_MAX_LENGTH);
    } else if (strcmp(line_buf, ":originActions") == 0) {
      location->origin_action_count = 0;
      while (strcmp(line_buf, ":endOriginActions") != 0) {
        fal_jbb_read_line(file, line_buf, line_buf_cap);

        if (location->origin_action_count >= FAL_LOCATION_ACTION_MAX_COUNT) {
          goto err;
        }

        if (strlen(line_buf) > FAL_ACTION_MAX_LENGTH) {
          goto err;
        }

        memcpy(location->origin_actions[location->origin_action_count++],
               line_buf, FAL_ACTION_MAX_LENGTH);
      }
    } else if (strcmp(line_buf, ":actions") == 0) {
      location->action_count = 0;
      while (strcmp(line_buf, ":endActions") != 0) {
        fal_jbb_read_line(file, line_buf, line_buf_cap);

        if (location->action_count >= FAL_LOCATION_ACTION_MAX_COUNT) {
          goto err;
        }

        if (strlen(line_buf) > FAL_ACTION_MAX_LENGTH) {
          goto err;
        }

        memcpy(location->actions[location->action_count++], line_buf,
               FAL_ACTION_MAX_LENGTH);
      }
    } else if (strcmp(line_buf, ":illustration") == 0) {
      for (int i = 0; i < FAL_ILLUSTRATION_HEIGHT; i++) {
        fal_jbb_parse_string(file, location->illustration[i],
                             FAL_ILLUSTRATION_WIDTH);
      }
    }

    if (strlen(location->origin_map) > 0) {
      for (int i = 0; i < location->origin_action_count; i++) {
        if (location->action_count >= FAL_LOCATION_ACTION_MAX_COUNT) {
          goto err;
        }
        strcpy(location->actions[location->action_count++],
               location->origin_actions[i]);
      }
    }
  }
  return true;
err:
  return false;
}

#define LOADER_PATH_BUF_CAP 4096
#define LOADER_LINE_BUF_CAP 1024
#define DECL_LOADER(asset_type_name, asset_type, asset_key, max_asset_count,   \
                    file_name)                                                 \
  static bool load_##asset_type_name##s(                                       \
      const char *module_identifier, asset_type *assets, int *asset_count) {   \
    char path_buf[LOADER_PATH_BUF_CAP] = {0};                                  \
    strlcat(path_buf, "data/", LOAD_ITEMS_PATH_BUF_CAPACITY);                  \
    strlcat(path_buf, module_identifier, LOAD_ITEMS_PATH_BUF_CAPACITY);        \
    int ret = strlcat(path_buf, "/" file_name, LOAD_ITEMS_PATH_BUF_CAPACITY);  \
    if (ret >= LOADER_PATH_BUF_CAP) {                                          \
      goto err;                                                                \
    }                                                                          \
    FILE *file = fopen(path_buf, "r");                                         \
    if (!file) {                                                               \
      goto close_file;                                                         \
    }                                                                          \
    char line_buf[LOADER_LINE_BUF_CAP];                                        \
    int asset_index = 0;                                                       \
    while (fal_jbb_read_line(file, line_buf, LOADER_LINE_BUF_CAP)) {           \
      if (asset_index >= (max_asset_count)) {                                  \
        FAL_PANIC("Too many " #asset_type_name "s !");                         \
      }                                                                        \
      asset_type *asset = &assets[asset_index];                                \
      if (strcmp(line_buf, (asset_key)) == 0) {                                \
        parse_##asset_type_name(file, line_buf, LOADER_LINE_BUF_CAP, asset);   \
        asset_index++;                                                         \
      }                                                                        \
    }                                                                          \
    if (ferror(file)) {                                                        \
      goto close_file;                                                         \
    }                                                                          \
    fclose(file);                                                              \
    return true;                                                               \
  close_file:                                                                  \
    fclose(file);                                                              \
  err:                                                                         \
    return false;                                                              \
  }

DECL_LOADER(item, struct fal_item, ":item", 64, "items.jbb")
DECL_LOADER(trigger, struct fal_trigger, ":trigger", 64, "triggers.jbb")
DECL_LOADER(mob, struct fal_mob, ":mob", 64, "mobs.jbb")
DECL_LOADER(npc, struct fal_npc, ":npc", 64, "npc.jbb")
DECL_LOADER(location, struct fal_location, ":location", 64, "locations.jbb")

#undef DECL_LOADER

static bool load_module(struct fal_engine *engine,
                        const char *module_identifier) {
  if (!load_items(module_identifier, engine->items, &engine->item_count)) {
    printf("Couldn't load items for module %s\n", module_identifier);
    goto err;
  }
  if (!load_mobs(module_identifier, engine->mobs, &engine->mob_count)) {
    printf("Couldn't load mobs for module %s\n", module_identifier);
    goto err;
  }
  if (!load_npcs(module_identifier, engine->npcs, &engine->npc_count)) {
    printf("Couldn't load npcs for module %s\n", module_identifier);
    goto err;
  }
  if (!load_triggers(module_identifier, engine->triggers,
                     &engine->trigger_count)) {
    printf("Couldn't load triggers for module %s\n", module_identifier);
    goto err;
  }
  if (!load_locations(module_identifier, engine->locations,
                      &engine->location_count)) {
    printf("Couldn't load locations for module %s\n", module_identifier);
    goto err;
  }
  return true;
err:
  return false;
}

struct fal_engine *fal_engine_create(void) {
  struct fal_engine *engine = malloc(sizeof(struct fal_engine));
  engine->console = fal_console_create();
  load_module(engine, "base");
  return engine;
}

static void display_main_menu(struct fal_engine *engine) {
  // clang-format off
  static const char ILLUSTRATION[18][80] = {
	"444444444444444444444444444444444444444444444444440EEEEEEEE000444444444444444444",
	"44444444444444444444444444444444444444440000444440EEFFFFFFEEEE044444444444444444",
	"4444440000444400044440444440444444444440FFF04444440FF0FFFFFFEEE04444444444444444",
	"4444400444444044404440444440444444444440FF044444440FFFFFF0FFF6604444444444444444",
	"4444404444440444044404444404444444444440FF004444440FFFFFFFFFFFF04444444444444444",
	"4444000044440004044404444404444444444400FFFF04444440FFFF0FFFFF000444444444444444",
	"44400444444044404440444440444444444440FFFFFF300000000F00FFFFF0033004444444444444",
	"4440444444404440444400004400004444440F7777F3333333333EEFFFFEEE333330000044444444",
	"4444444444444444444444444444444444440FFFFFF33333333333EEEEEEE3333333333004444444",
	"4444444444444444444444444444444444440F7777F333333333333EEEEE33333333333300044444",
	"44444444400444440044440044444444444440FFFFFF333300003333EE3333333333333333004444",
	"444444444004444000444400444444444444440000FF330044403333EE3333330000033333304444",
	"44444444004444400400400444444444444444444400004444403333EE3333330444033333304444",
	"44444440044444004400000444444444444444444444444444403333EE3333330440333333304444",
	"4444444004444400444000444444444444444444444444444440EE333EE3333E0403333333004444",
	"44444444444444444444444444444444444444444444444444440EEEEEEEEEEE0FF3333330044444",
	"44444444444444444444444444444444444444444444444444440EEEEEEEEEE3FFFFF33000444444",
	"4444444444444444444444444444444444444444444444444444033333333330FFFFFF0044444444",
  };
  // clang-format on

  fal_console_ui_display_illustration(engine->console, ILLUSTRATION, 18, 0);
}

void fal_engine_start(struct fal_engine *engine) { display_main_menu(engine); }

void fal_engine_destroy(struct fal_engine *engine) {
  fal_console_destroy(engine->console);
  free(engine);
}
