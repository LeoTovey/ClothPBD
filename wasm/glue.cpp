
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

// WASMSim

WASMSim* EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_WASMSim_1(int n_particles) {
  return new WASMSim(n_particles);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_InitPosition_4(WASMSim* self, int index, float x, float y, float z) {
  self->InitPosition(index, x, y, z);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_InitSim_9(WASMSim* self, int sim_type, float width, float height, int x_segments, int y_segments, float k, float mass, float damping, float gravity) {
  self->InitSim(sim_type, width, height, x_segments, y_segments, k, mass, damping, gravity);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_Step_2(WASMSim* self, float dt, int max_steps) {
  self->Step(dt, max_steps);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_GetPositionX_1(WASMSim* self, int index) {
  return self->GetPositionX(index);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_GetPositionY_1(WASMSim* self, int index) {
  return self->GetPositionY(index);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_GetPositionZ_1(WASMSim* self, int index) {
  return self->GetPositionZ(index);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_SetFixedNodeVelocity_3(WASMSim* self, float v0, float v1, float v2) {
  self->SetFixedNodeVelocity(v0, v1, v2);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_AddSphere_1(WASMSim* self, float r) {
  self->AddSphere(r);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_UpdateSphere_3(WASMSim* self, float x, float y, float z) {
  self->UpdateSphere(x, y, z);
}

const char* EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_Print_0(WASMSim* self) {
  return self->Print();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_get_a_0(WASMSim* self) {
  return self->a;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim_set_a_1(WASMSim* self, int arg0) {
  self->a = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_WASMSim___destroy___0(WASMSim* self) {
  delete self;
}

// Foo

Foo* EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo_Foo_0() {
  return new Foo();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo_getVal_0(Foo* self) {
  return self->getVal();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo_setVal_1(Foo* self, int v) {
  self->setVal(v);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo___destroy___0(Foo* self) {
  delete self;
}

}

