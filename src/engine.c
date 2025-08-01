#include "console.h"
#include "engine.h"
#include "jbb.h"
#include "panic.h"
#include "platform.h"
#include "str.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAL_DESCRIPTION_MAX_LENGTH 64
#define FAL_ILLUSTRATION_WIDTH 80
#define FAL_ILLUSTRATION_HEIGHT 10

struct fal_trigger {
  char id[FAL_ID_MAX_LENGTH];
  char actions[FAL_ACTION_MAX_LENGTH][FAL_ACTION_MAX_COUNT];
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

struct fal_location {
  char id[FAL_ID_MAX_LENGTH];
  char name[FAL_NAME_MAX_LENGTH];
  char description_day[FAL_DESCRIPTION_MAX_LENGTH];
  char description_night[FAL_DESCRIPTION_MAX_LENGTH];
  char illustration[FAL_ILLUSTRATION_WIDTH][FAL_ILLUSTRATION_HEIGHT];
  char actions[FAL_ACTION_MAX_LENGTH][FAL_ACTION_MAX_COUNT];
  int action_count;
  char origin_map[FAL_ID_MAX_LENGTH];
  char origin_actions[FAL_ACTION_MAX_LENGTH][FAL_ACTION_MAX_COUNT];
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

struct fal_inventory_cell {
  char item_id[FAL_ID_MAX_LENGTH];
  int quantity;
  struct fal_inventory_cell *next;
  struct fal_inventory_cell *previous;
};

#define FAL_PLAYER_NAME_MAX_LENGTH 30
#define FAL_PLAYER_CHOICES_CAPACITY 64
#define FAL_PLAYER_MAX_HP 100
#define FAL_PLAYER_SPEED 100
#define FAL_PLAYER_STRENGTH 1
#define FAL_PLAYER_HUNGER 100
#define FAL_PLAYER_HUNGER_LOSS_PER_HOUR 1
// Level of hunger below which player starts to lose HP
#define FAL_PLAYER_HUNGER_FLOOR 30
#define FAL_PLAYER_HP_LOSS_HUNGRY 1
struct fal_player {
  char name[FAL_PLAYER_NAME_MAX_LENGTH];
  char location[FAL_ID_MAX_LENGTH];
  int hp;
  int max_hp;
  int speed;
  int strength;
  int time;
  int hunger;
  int money;
  char tool[FAL_ID_MAX_LENGTH];
  char armor[FAL_ID_MAX_LENGTH];
  struct fal_action choices[FAL_PLAYER_CHOICES_CAPACITY];
  int choice_count;
  struct fal_inventory_cell *inventory;
};

void fal_player_init(struct fal_player *player, const char *name) {
  strcpy(player->name, name);
  strcpy(player->location, "universe");
  player->hp = FAL_PLAYER_MAX_HP;
  player->max_hp = FAL_PLAYER_MAX_HP;
  player->speed = FAL_PLAYER_SPEED;
  player->strength = FAL_PLAYER_STRENGTH;
  player->hunger = FAL_PLAYER_HUNGER;
  player->inventory = NULL;
  player->time = 12;
  player->money = 0;
  strcpy(player->armor, "");
  strcpy(player->tool, "");
}

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
  struct fal_player player;
  fal_console *console;
  int item_count;
  int trigger_count;
  int mob_count;
  int npc_count;
  int location_count;
};

#define DECL_FIND_BY_ID_FNS(T, name)                                           \
  static T *find_##name(T *array, int count,                                   \
                        const char id[FAL_ID_MAX_LENGTH]) {                    \
    for (int i = 0; i < count; i++) {                                          \
      if (strcmp(array[i].id, id) == 0) {                                      \
        return &array[i];                                                      \
      }                                                                        \
    }                                                                          \
    return NULL;                                                               \
  }

DECL_FIND_BY_ID_FNS(struct fal_location, location)
DECL_FIND_BY_ID_FNS(struct fal_item, item)
DECL_FIND_BY_ID_FNS(struct fal_npc, npc)

static const char *get_season(struct fal_engine *engine) {
  int time = engine->player.time;
  if (time % (28 * 24) < 7 * 24) {
    return "Spring";
  } else if (time % (28 * 24) < 14 * 24) {
    return "Summer";
  } else if (time % (28 * 24) < 21 * 24) {
    return "Fall";
  } else {
    return "Winter";
  }
}

static void
execute_actions(struct fal_engine *engine,
                const char actions[FAL_ACTION_MAX_LENGTH][FAL_ACTION_MAX_COUNT],
                int action_count);

static void action_goto(struct fal_engine *engine, char *map_id, int duration) {
  struct fal_player *player = &engine->player;
  strcpy(player->location, map_id);
  player->time += duration;
  if (strcmp(get_season(engine), "Winter") == 0) {
    player->time += duration; // double the snow, double the fun!
  }
  player->choice_count = 0;
}

static struct fal_inventory_cell *
find_inventory_cell(struct fal_inventory_cell **inventory,
                    const char *item_id) {
  if (*inventory == NULL) {
    return NULL;
  }

  struct fal_inventory_cell *cell = *inventory;
  while (strcmp(cell->item_id, item_id) != 0 && cell != NULL) {
    cell = cell->next;
  }

  return cell;
}

static void add_item_to_inventory(struct fal_inventory_cell **inventory,
                                  char item_id[FAL_ID_MAX_LENGTH],
                                  int quantity) {
  struct fal_inventory_cell *cell = find_inventory_cell(inventory, item_id);
  if (cell) {
    cell->quantity += quantity;
  } else {
    struct fal_inventory_cell *new_cell =
        malloc(sizeof(struct fal_inventoy_cell *));
    strcpy(new_cell->item_id, item_id);
    new_cell->quantity = quantity;
    if (*inventory == NULL) {
      new_cell->next = NULL;
      new_cell->previous = NULL;
      *inventory = new_cell;
    } else {
      new_cell->next = *inventory;
      new_cell->previous = NULL;
      (*inventory)->previous = new_cell;
      *inventory = new_cell;
    }
  }
}

static void action_harvest(struct fal_engine *engine, char *item_id,
                           int quantity, int duration) {
  struct fal_item *harvested_item =
      find_item(engine->items, engine->item_count, item_id);
  struct fal_player *player = &engine->player;
  int final_quantity = 0;
  if (strlen(player->tool) > 0) {
    struct fal_item *tool_item =
        find_item(engine->items, engine->item_count, player->tool);
    if (strcmp(tool_item->category, "tool") == 0) {
      final_quantity = tool_item->loot_modifier + quantity;
    }
  } else {
    final_quantity = quantity;
  }
  player->time += duration;
  add_item_to_inventory(&player->inventory, item_id, final_quantity);
  fal_console_ui_display_notice_pop(engine->console, "You harvested %dx %s.",
                                    final_quantity, harvested_item->name);
}

#define FAL_NPC_MESSAGE_BUFFER_CAPACITY                                        \
  (FAL_NAME_MAX_LENGTH + FAL_DESCRIPTION_MAX_LENGTH + 4)
static void action_trade(struct fal_engine *engine,
                         const char npc_id[FAL_ID_MAX_LENGTH]) {
  struct fal_npc *npc = find_npc(engine->npcs, engine->npc_count, npc_id);
  char message[FAL_NPC_MESSAGE_BUFFER_CAPACITY] = {0};
  snprintf(message, FAL_NPC_MESSAGE_BUFFER_CAPACITY, "%s: \\%s", npc->name,
           npc->greetings);

  char splits[1000][80] = {};
  int split_count = fal_split_displayed_text(message, '\\', splits);
  fal_console_ui_display_interface(
      engine->console, FAL_UI_POSITION_DESCRIPTION_BOX, FAL_BORDER_SINGLE,
      splits, split_count, false);

  static const char *PROMPT_MESSAGE = "What do you want to do?";

  struct fal_action buy_action = {.name = "Buy", .key = "b"};
  sprintf(buy_action.actions[0], "buy %s", npc_id);
  buy_action.action_count = 1;

  struct fal_action sell_action = {.name = "Sell", .key = "s"};
  sprintf(sell_action.actions[0], "sell%s", npc_id);
  sell_action.action_count = 1;

  struct fal_action cancel_action = {.name = "Cancel", .key = "a"};

  struct fal_action actions[] = {buy_action, sell_action, cancel_action};

  char actions_to_run[FAL_ACTION_MAX_LENGTH][FAL_ACTION_MAX_COUNT];
  int action_to_run_count = fal_console_ui_display_pop(
      engine->console, PROMPT_MESSAGE, actions,
      sizeof(actions) / sizeof(actions[0]), actions_to_run);
  execute_actions(engine, actions_to_run, action_to_run_count);
}

static void
execute_actions(struct fal_engine *engine,
                const char actions[FAL_ACTION_MAX_LENGTH][FAL_ACTION_MAX_COUNT],
                int action_count) {
  for (int i = 0; i < action_count; i++) {
    char current_action[FAL_ACTION_MAX_LENGTH];
    strcpy(current_action, actions[i]);
    char *action = strtok(current_action, " ");
    if (strcmp(action, "goto") == 0) {
      char *map_id = strtok(NULL, " ");
      char *duration_str = strtok(NULL, " ");
      int duration = (int)strtol(duration_str, NULL, 10);
      action_goto(engine, map_id, duration);
    } else if (strcmp(action, "harvest") == 0) {
      char *item_id = strtok(NULL, " ");
      char *quantity_str = strtok(NULL, " ");
      char *duration_str = strtok(NULL, " ");
      int quantity = strtol(quantity_str, NULL, 10);
      int duration = strtol(duration_str, NULL, 10);
      action_harvest(engine, item_id, quantity, duration);
    } else if (strcmp(action, "trade") == 0) {
      char *npc_id = strtok(NULL, " ");
      action_trade(engine, npc_id);
    } else if (strcmp(action, "buy") == 0) {
      // TODO
    } else if (strcmp(action, "sell") == 0) {
      // TODO
    } else if (strcmp(action, "time") == 0) {
      // TODO
    } else if (strcmp(action, "set") == 0) {
      // TODO
    } else if (strcmp(action, "relset") == 0) {
      // TODO
    } else if (strcmp(action, "wear") == 0) {
      // TODO
    } else if (strcmp(action, "equip") == 0) {
      // TODO
    } else if (strcmp(action, "use") == 0) {
      // TODO
    } else if ((strcmp(action, "spawn") == 0) ||
               (strcmp(action, "spoon") == 0)) {
      // TODO
    } else if (strcmp(action, "trigger") == 0) {
      // TODO
    } else if (strcmp(action, "give") == 0) {
      // TODO
    } else if (strcmp(action, "remove") == 0) {
      // TODO
    } else if (strcmp(action, "container") == 0) {
      // TODO
    } else if (strcmp(action, "transfert") == 0) {
      // TODO
    } else if (strcmp(action, "craft") == 0) {
      // TODO
    } else if (strcmp(action, "addchoice") == 0) {
      // TODO
    } else if (strcmp(action, "pop") == 0) {
      // TODO
    } else if (strcmp(action, "addpopchoice") == 0) {
      // TODO
    } else if (strcmp(action, "clearpopchoice") == 0) {
      // TODO
    } else if (strcmp(action, "askpop") == 0) {
      // TODO
    } else if (strcmp(action, "prob") == 0) {
      // TODO
    } else if (strcmp(action, "if") == 0) {
      // TODO
    } else if (strcmp(action, "%") != 0 && strcmp(action, "") != 0) {
      // TODO
    }
  }
}

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

        if (trigger->action_count >= FAL_ACTION_MAX_COUNT) {
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

        if (location->origin_action_count >= FAL_ACTION_MAX_COUNT) {
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

        if (location->action_count >= FAL_ACTION_MAX_COUNT) {
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
        if (location->action_count >= FAL_ACTION_MAX_COUNT) {
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
    *asset_count = asset_index;                                                \
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
  static const char ILLUSTRATION[][80] = {
"11111111111111111111111111111111111111111111111111111111111111111111111111111111",
"11111111111111111111111111111111111111111111111111111111111111111111111111111111",
"11111100001111000111101111101111111111111111111111119999999999111110000000001111",
"11111001111110111011101111101111111111111111111119999999999111110000000000001111",
"11111011111101110111011111011111111111111111111111111111111111000001111111111111",
"11110000111100010111011111011111111111111111119999999999111111000111111111111111",
"11100111111011101110111110111111111111111111119999999999111110011111111111111111",
"11101111111011101111000011000011111111111111111111111111111110011111111111111111",
"11111111111111111111111111111111111111111111119999999999111110011111111111111111",
"11111111111111111111111111111111111111111111199999999991111100111111111111111111",
"11111111100111110011110011111111111111111111111111111111111100111111111111111111",
"11111111100111100011110011111111111111111111199999999991111100111111111111111111",
"11111111001111100100100111111111111111111111199999999991111100001111111111111111",
"11111110011111001100000111111111111111111111111111111111111110000000000011111111",
"11111110011111001110001111111111111111111111111999999999911111000000000011111111",
"11111111111111111111111111111111111111111111111111111111111111111111111111111111",
"11111111111111111111111111111111111111111111111111111111111111111111111111111111",
"11111111111111111111111111111111111111111111111111111111111111111111111111111111",
  };
  // clang-format on

  fal_console_clear(engine->console);
  fal_console_ui_display_illustration(
      engine->console, ILLUSTRATION,
      sizeof(ILLUSTRATION) / sizeof(ILLUSTRATION[0]), 0);

  static const char MAIN_MENU_CONTENT[][80] = {
      "[n] Start a new game", "[l] Load game", "[m] Load module", "[a] About",
      "[q] Exit"};

  fal_console_ui_display_interface(
      engine->console, FAL_UI_POSITION_BOTTOM, FAL_BORDER_SINGLE,
      MAIN_MENU_CONTENT,
      sizeof(MAIN_MENU_CONTENT) / sizeof(MAIN_MENU_CONTENT[0]), false);
}

#define FAL_MAX(a, b) ((a) > (b) ? (a) : (b))
static void main_loop(struct fal_engine *engine) {
  struct fal_player *player = &engine->player;
  int time_of_last_action = player->time;
  bool quit = false;
  enum fal_key choice;
  while (player->hp > 0 && choice != FAL_KEY_ESCAPE && choice != FAL_KEY_Q &&
         !quit) {
    if (player->time > time_of_last_action) {
      player->hunger =
          FAL_MAX(player->hunger - (player->time - time_of_last_action) *
                                       FAL_PLAYER_HUNGER_LOSS_PER_HOUR,
                  0);
    }

    if (player->hunger < FAL_PLAYER_HUNGER_FLOOR) {
      player->hp = FAL_MAX(player->hp - (player->time - time_of_last_action) *
                                            FAL_PLAYER_HP_LOSS_HUNGRY,
                           0);
    }

    time_of_last_action = player->time;
    if (player->hp > 0) {
      player->choice_count = 0;
      // Execute map actions (random encounter or such)
      struct fal_location *location = find_location(
          engine->locations, engine->location_count, player->location);
      execute_actions(engine, location->actions, location->action_count);
    }
  }
}

static void start_new_game(struct fal_engine *engine) {
  static const char POP_BOX_CONTENT[][80] = {"What is your name?"};
  fal_console_ui_display_interface(
      engine->console, FAL_UI_POSITION_POP_BOX, FAL_BORDER_SINGLE,
      POP_BOX_CONTENT, sizeof(POP_BOX_CONTENT) / sizeof(POP_BOX_CONTENT[0]),
      true);

  char player_name[FAL_PLAYER_NAME_MAX_LENGTH];
  fal_console_read_line(engine->console, player_name,
                        FAL_PLAYER_NAME_MAX_LENGTH);
  fal_player_init(&engine->player, player_name);
  main_loop(engine);
}

void fal_engine_start(struct fal_engine *engine) {
  bool running = true;
  while (running) {
    display_main_menu(engine);
    enum fal_key key = fal_platform_read_key();
    if (key == FAL_KEY_ESCAPE || key == FAL_KEY_Q) {
      running = false;
    } else if (key == FAL_KEY_N) {
      start_new_game(engine);
    } else if (key == FAL_KEY_L) {
      // load game
    } else if (key == FAL_KEY_M) {
      // load module
    } else if (key == FAL_KEY_A) {
      fal_console_ui_display_notice_pop(engine->console,
                                        " Software made by @lisible\\          "
                                        "     ~~ @sliya\\              "
                                        " ~~ @metzg");
    }
  }
}

void fal_engine_destroy(struct fal_engine *engine) {
  fal_console_destroy(engine->console);
  free(engine);
}
