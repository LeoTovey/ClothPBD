import './style/main.css';
import * as THREE from 'three';

import { GUI } from 'three/examples/jsm/libs/lil-gui.module.min.js';
import Stats from 'three/examples/jsm/libs/stats.module.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import Module from './PBD.js';

// index.ts
// declare var PBD;
// use PBD as you wish


// import('./PBD.js').then((Module) => {

// 	console.log();
// }).catch((error) => {
// 	console.error('Error importing module:', error);
// });
// let PBD: any;



// import Module from './PBD.js'
// let PBD = Module();
// console.log(PBD);
// var f = PBD.Foo();


// async function loadModule() {
// 	let PBD = Module();
// 	console.log(PBD);
// 	var f = PBD.Foo();
// 	// 这里可以继续执行后续的代码
// }

// loadModule();
// declare global {
// 	interface Window {
// 		main: () => void; // 声明window对象上有一个main函数
// 	}
// }


let PBDM, f;
// const Module = require('./PBD.js');
// console.log(PBD);
// var PBD;
// window.main = function main() 
// {
// 	Module().then(function (mymod) 
// 	{
// 		PBD = mymod;
// 		console.log("123");
// 		console.log(mymod);
// 	});
// }


Module().then(function (PBD) 
{
	console.log(PBD);
	PBDM = PBD;
	f = new PBD.Foo();
	console.log(f);
});




// import Module from './PBD.js'
// const mymod = Module();
// //var f = new mymod.Foo();
// console.log(mymod.Foo());



let scene, renderer, camera, stats, container;
let cloth;
const controls =
{
	enabledShadow: true,
}

init();
animate();



console.log(window);

function init() {
	container = document.createElement('div');
	document.body.appendChild(container);

	scene = new THREE.Scene();
	scene.background = new THREE.Color(0xe0e0e0);
	scene.fog = new THREE.Fog(0xe0e0e0, 20, 100);

	renderer = new THREE.WebGLRenderer({ antialias: true });
	renderer.setPixelRatio(window.devicePixelRatio);
	renderer.setSize(window.innerWidth, window.innerHeight);
	renderer.shadowMap.enabled = true;

	camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 0.25, 100);
	camera.position.set(- 5, 3, 10);
	camera.lookAt(0, 2, 0);

	// light
	const hemiLight = new THREE.HemisphereLight(0xffffff, 0x8d8d8d, 3);
	hemiLight.position.set(0, 20, 0);
	scene.add(hemiLight);

	const dirLight = new THREE.DirectionalLight(0xffffff, 3);
	dirLight.position.set(0, 20, 10);
	dirLight.castShadow = true;
	// dirLight.shadow.camera.top = 2;
	// dirLight.shadow.camera.bottom = - 2;
	// dirLight.shadow.camera.left = - 2;
	// dirLight.shadow.camera.right = 2;
	// dirLight.shadow.camera.near = 0.1;
	// dirLight.shadow.camera.far = 40;
	scene.add(dirLight);

	window.addEventListener('resize', onWindowResize);

	// stats
	stats = Stats();
	container.appendChild(stats.dom);
	container.appendChild(renderer.domElement);

	// model

	// ground
	const mesh = new THREE.Mesh(new THREE.PlaneGeometry(2000, 2000), new THREE.MeshPhongMaterial({ color: 0xcbcbcb, depthWrite: false }));
	mesh.rotation.x = - Math.PI / 2;
	mesh.receiveShadow = true;
	scene.add(mesh);

	const grid = new THREE.GridHelper(200, 40, 0x000000, 0x000000);
	console.log(grid.material)
	if (Array.isArray(grid.material)) {
		// material is an array, loop through it
		for (let mat of grid.material) {
			mat.opacity = 0.2;
			mat.transparent = true;
		}
	}
	else {
		// material is not an array, set opacity directly
		grid.material.opacity = 0.2;
		grid.material.transparent = true;
	}


	scene.add(grid);

	// radius ?: number,
	// 	widthSegments ?: number,
	// 	heightSegments ?: number,
	// 	phiStart ?: number,
	// 	phiLength ?: number,
	// 	thetaStart ?: number,
	// 	thetaLength ?: number,

	// interactive sphere
	const sphereGeometry = new THREE.SphereGeometry(0.4);
	const sphereMaterial = new THREE.MeshStandardMaterial({ color: 0xFFFFFF });
	sphereMaterial.roughness = 1.0;
	console.log(sphereMaterial);
	const sphere = new THREE.Mesh(sphereGeometry, sphereMaterial);



	sphere.position.set(0, 1, 1);
	scene.add(sphere);

	//cloth
	// Cloth graphic object
	const clothWidth = 4;
	const clothHeight = 3;
	const clothNumSegmentsZ = clothWidth * 5;
	const clothNumSegmentsY = clothHeight * 5;
	const clothPos = new THREE.Vector3(0, -1, 0);
	const clothMaterial = new THREE.MeshStandardMaterial({ color: 0xFFFFFF, side: THREE.DoubleSide });
	const clothGeometry = new THREE.PlaneGeometry(clothWidth, clothHeight, clothNumSegmentsZ, clothNumSegmentsY);
	cloth = new THREE.Mesh(clothGeometry, clothMaterial);

	clothGeometry.rotateX(Math.PI * 0.5);
	//clothGeometry.translate(0, 10.0, 0.0);
	//clothGeometry.translate(clothPos.x, clothPos.y + clothHeight * 0.5, clothPos.z - clothWidth * 0.5);
	cloth.castShadow = true;
	cloth.receiveShadow = true;
	cloth.position.set(0.0, 2.0, 0.0);
	scene.add(cloth);

	new THREE.TextureLoader().load('./images.jpg', function (texture) {

		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
		cloth.material.map = texture;
		cloth.material.needsUpdate = true;
		create_ui();

	});

	// 监听鼠标事件
	document.addEventListener('mousedown', onMouseDown);
	document.addEventListener('mouseup', onMouseUp);
	document.addEventListener('mousemove', onMouseMove);
}

function onWindowResize() {
	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();
	renderer.setSize(window.innerWidth, window.innerHeight);
}

function create_ui() {
	const panel = new GUI({ width: 310 });
	const shadowCtrl = panel.add(controls, 'enabledShadow').name('Enable Shadow')
	shadowCtrl.onChange(
		function () {

			console.log(f);
			console.log(f.getVal());
			console.log(PBDM);
			
			renderer.shadowMap.enabled = controls.enabledShadow;
		}
	);
}


function animate() {
	requestAnimationFrame(animate);

	cloth.rotation.x += 0.01;

	renderer.render(scene, camera);
	stats.update();
}

function onMouseDown() {

}

function onMouseMove() {

}

function onMouseUp() {

}



