
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

ClothSim* EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_ClothSim_2(int x_segments, int y_segments) {
  return new ClothSim(x_segments, y_segments);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim_Step_1(ClothSim* self, float dt) {
  return self->Step(dt);
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

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ClothSim___destroy___0(ClothSim* self) {
  delete self;
}

}

