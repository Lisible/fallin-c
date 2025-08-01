#ifndef FAL_ENGINE_H
#define FAL_ENGINE_H

#define FAL_ID_MAX_LENGTH 64
#define FAL_NAME_MAX_LENGTH 64
#define FAL_ACTION_MAX_LENGTH 1024
#define FAL_ACTION_MAX_COUNT 32
#define FAL_ACTION_ACTIONS_CAPACITY 64
struct fal_action {
  char key[FAL_ID_MAX_LENGTH];
  char name[FAL_NAME_MAX_LENGTH];
  char actions[FAL_ACTION_ACTIONS_CAPACITY][FAL_ACTION_MAX_LENGTH];
  int action_count;
};

typedef struct fal_engine fal_engine;
struct fal_engine *fal_engine_create(void);
void fal_engine_start(struct fal_engine *engine);
void fal_engine_destroy(struct fal_engine *engine);

#endif // FAL_ENGINE_H
