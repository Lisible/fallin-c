#ifndef FAL_ENGINE_H
#define FAL_ENGINE_H

typedef struct fal_engine fal_engine;
struct fal_engine *fal_engine_create(void);
void fal_engine_start(struct fal_engine *engine);
void fal_engine_destroy(struct fal_engine *engine);

#endif // FAL_ENGINE_H
