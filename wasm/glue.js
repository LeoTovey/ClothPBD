
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_webidl_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_webidl_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_webidl_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = (len + 7) & -8; // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_webidl_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy(array, view, offset) {
    offset >>>= 0;
    var bytes = view.BYTES_PER_ELEMENT;
    switch (bytes) {
      case 2: offset >>>= 1; break;
      case 4: offset >>>= 2; break;
      case 8: offset >>>= 3; break;
    }
    for (var i = 0; i < array.length; i++) {
      view[offset + i] = array[i];
    }
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}


// VoidPtr
/** @suppress {undefinedVars, duplicate} @this{Object} */function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
// WASMSim
/** @suppress {undefinedVars, duplicate} @this{Object} */function WASMSim(n_particles) {
  if (n_particles && typeof n_particles === 'object') n_particles = n_particles.ptr;
  this.ptr = _emscripten_bind_WASMSim_WASMSim_1(n_particles);
  getCache(WASMSim)[this.ptr] = this;
};;
WASMSim.prototype = Object.create(WrapperObject.prototype);
WASMSim.prototype.constructor = WASMSim;
WASMSim.prototype.__class__ = WASMSim;
WASMSim.__cache__ = {};
Module['WASMSim'] = WASMSim;

WASMSim.prototype['InitPosition'] = WASMSim.prototype.InitPosition = /** @suppress {undefinedVars, duplicate} @this{Object} */function(index, x, y, z) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_WASMSim_InitPosition_4(self, index, x, y, z);
};;

WASMSim.prototype['InitMassSpring'] = WASMSim.prototype.InitMassSpring = /** @suppress {undefinedVars, duplicate} @this{Object} */function(width, height, x_segments, y_segments, k, mass, damping, gravity) {
  var self = this.ptr;
  if (width && typeof width === 'object') width = width.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  if (x_segments && typeof x_segments === 'object') x_segments = x_segments.ptr;
  if (y_segments && typeof y_segments === 'object') y_segments = y_segments.ptr;
  if (k && typeof k === 'object') k = k.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (damping && typeof damping === 'object') damping = damping.ptr;
  if (gravity && typeof gravity === 'object') gravity = gravity.ptr;
  _emscripten_bind_WASMSim_InitMassSpring_8(self, width, height, x_segments, y_segments, k, mass, damping, gravity);
};;

WASMSim.prototype['Step'] = WASMSim.prototype.Step = /** @suppress {undefinedVars, duplicate} @this{Object} */function(dt, max_steps) {
  var self = this.ptr;
  if (dt && typeof dt === 'object') dt = dt.ptr;
  if (max_steps && typeof max_steps === 'object') max_steps = max_steps.ptr;
  _emscripten_bind_WASMSim_Step_2(self, dt, max_steps);
};;

WASMSim.prototype['GetPositionX'] = WASMSim.prototype.GetPositionX = /** @suppress {undefinedVars, duplicate} @this{Object} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return _emscripten_bind_WASMSim_GetPositionX_1(self, index);
};;

WASMSim.prototype['GetPositionY'] = WASMSim.prototype.GetPositionY = /** @suppress {undefinedVars, duplicate} @this{Object} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return _emscripten_bind_WASMSim_GetPositionY_1(self, index);
};;

WASMSim.prototype['GetPositionZ'] = WASMSim.prototype.GetPositionZ = /** @suppress {undefinedVars, duplicate} @this{Object} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return _emscripten_bind_WASMSim_GetPositionZ_1(self, index);
};;

WASMSim.prototype['SetFixedNodeVelocity'] = WASMSim.prototype.SetFixedNodeVelocity = /** @suppress {undefinedVars, duplicate} @this{Object} */function(v0, v1, v2) {
  var self = this.ptr;
  if (v0 && typeof v0 === 'object') v0 = v0.ptr;
  if (v1 && typeof v1 === 'object') v1 = v1.ptr;
  if (v2 && typeof v2 === 'object') v2 = v2.ptr;
  _emscripten_bind_WASMSim_SetFixedNodeVelocity_3(self, v0, v1, v2);
};;

WASMSim.prototype['AddSphere'] = WASMSim.prototype.AddSphere = /** @suppress {undefinedVars, duplicate} @this{Object} */function(r) {
  var self = this.ptr;
  if (r && typeof r === 'object') r = r.ptr;
  _emscripten_bind_WASMSim_AddSphere_1(self, r);
};;

WASMSim.prototype['UpdateSphere'] = WASMSim.prototype.UpdateSphere = /** @suppress {undefinedVars, duplicate} @this{Object} */function(x, y, z) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_WASMSim_UpdateSphere_3(self, x, y, z);
};;

WASMSim.prototype['Print'] = WASMSim.prototype.Print = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_WASMSim_Print_0(self));
};;

  WASMSim.prototype['get_a'] = WASMSim.prototype.get_a = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  return _emscripten_bind_WASMSim_get_a_0(self);
};
    WASMSim.prototype['set_a'] = WASMSim.prototype.set_a = /** @suppress {undefinedVars, duplicate} @this{Object} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_WASMSim_set_a_1(self, arg0);
};
    /** @suppress {checkTypes} */
    Object.defineProperty(WASMSim.prototype, 'a', { get: WASMSim.prototype.get_a, set: WASMSim.prototype.set_a });
  WASMSim.prototype['__destroy__'] = WASMSim.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_WASMSim___destroy___0(self);
};
// Foo
/** @suppress {undefinedVars, duplicate} @this{Object} */function Foo() {
  this.ptr = _emscripten_bind_Foo_Foo_0();
  getCache(Foo)[this.ptr] = this;
};;
Foo.prototype = Object.create(WrapperObject.prototype);
Foo.prototype.constructor = Foo;
Foo.prototype.__class__ = Foo;
Foo.__cache__ = {};
Module['Foo'] = Foo;

Foo.prototype['getVal'] = Foo.prototype.getVal = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  return _emscripten_bind_Foo_getVal_0(self);
};;

Foo.prototype['setVal'] = Foo.prototype.setVal = /** @suppress {undefinedVars, duplicate} @this{Object} */function(v) {
  var self = this.ptr;
  if (v && typeof v === 'object') v = v.ptr;
  _emscripten_bind_Foo_setVal_1(self, v);
};;

  Foo.prototype['__destroy__'] = Foo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_Foo___destroy___0(self);
};