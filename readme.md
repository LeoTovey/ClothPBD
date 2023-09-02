

# Template
<https://leotovey.github.io/ClothPBD/>
## Install
This project uses [node](http://nodejs.org) and [npm](https://npmjs.com). Go check them out if you don't have them locally installed.

## Usage


``` bash
# Install dependencies (only for first time)
npm i

# Run the local dev server
npm run dev

# Build for production in the dist/ directory
npm run build
```


<html lang="en">
	<head>
		<title>Ammo.js softbody cloth demo</title>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0">
		<link type="text/css" rel="stylesheet" href="main.css">
		<style>
			body {
				color: #333;
			}
		</style>
	</head>
	<body>
		<div id="info">Ammo.js physics soft body cloth demo<br>Press Q or A to move the arm.</div>
		<div id="container"></div>

		<script src="jsm/libs/ammo.wasm.js"></script>

		<!-- Import maps polyfill -->
		<!-- Remove this when import maps will be widely supported -->
		<script async src="https://unpkg.com/es-module-shims@1.6.3/dist/es-module-shims.js"></script>

		<script type="importmap">
			{
				"imports": {
					"three": "../build/three.module.js",
					"three/addons/": "./jsm/"
				}
			}
		</script>

		<script type="module">

			import * as THREE from 'three';

			import Stats from 'three/addons/libs/stats.module.js';

			import { OrbitControls } from 'three/addons/controls/OrbitControls.js';

			// Graphics variables
			let container, stats;
			let camera, controls, scene, renderer;
			let textureLoader;
			const clock = new THREE.Clock();

			// Physics variables
			const gravityConstant = - 9.8;
			let physicsWorld;
			const rigidBodies = [];
			const margin = 0.05;
			let hinge;
			let cloth;
			let transformAux1;

			let armMovement = 0;


		</script>

	</body>
</html>

