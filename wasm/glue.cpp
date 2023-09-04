
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


EM_JS(void, array_bounds_check_error, (size_t idx, size_t size), {
  throw 'Array index ' + idx + ' out of bounds: [0,' + size + ')';
});

static void array_bounds_check(size_t array_size, size_t array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    array_bounds_check_error(array_idx, array_size);
  }
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// ClothSim

ClothSim* EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_ClothSim_4(float width, float height, int x_segments, int y_segments) {
  return new ClothSim(width, height, x_segments, y_segments);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_Step_1(ClothSim* self, float dt) {
  self->Step(dt);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_GetPositionX_1(ClothSim* self, int index) {
  return self->GetPositionX(index);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_GetPositionY_1(ClothSim* self, int index) {
  return self->GetPositionY(index);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_GetPositionZ_1(ClothSim* self, int index) {
  return self->GetPositionZ(index);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_SetPosition_4(ClothSim* self, int index, float x, float y, float z) {
  self->SetPosition(index, x, y, z);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_UpdateSphere_4(ClothSim* self, float x, float y, float z, float r) {
  self->UpdateSphere(x, y, z, r);
}

const char* EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_Print_0(ClothSim* self) {
  return self->Print();
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_get_k_0(ClothSim* self) {
  return self->k;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_set_k_1(ClothSim* self, float arg0) {
  self->k = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_get_node_mass_0(ClothSim* self) {
  return self->node_mass;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_set_node_mass_1(ClothSim* self, float arg0) {
  self->node_mass = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_get_damping_0(ClothSim* self) {
  return self->damping;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_set_damping_1(ClothSim* self, float arg0) {
  self->damping = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_get_gravity_0(ClothSim* self) {
  return self->gravity;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_set_gravity_1(ClothSim* self, float arg0) {
  self->gravity = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_get_dt_0(ClothSim* self) {
  return self->dt;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_set_dt_1(ClothSim* self, float arg0) {
  self->dt = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim___destroy___0(ClothSim* self) {
  delete self;
}

}

