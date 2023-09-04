import './style/main.css';
import * as THREE from 'three';

import { GUI } from 'three/examples/jsm/libs/lil-gui.module.min.js';
import Stats from 'three/examples/jsm/libs/stats.module.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import Module from './PBD.js';




let PBD;
let scene, renderer, camera, stats, container;
let cloth, PBDCloth, dirLight, sphere;
var clock = new THREE.Clock();

// const dt = 1e-3;
const k = 8.0;
const damping = 0.2;
const mass = 1.0;
const gravity = 0.01;
const sphere_radius = 0.4;

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
	const hemiLight = 
	new THREE.HemisphereLight(0xffffff, 0x8d8d8d, 3);
	hemiLight.position.set(0, 20, 0);
	scene.add(hemiLight);

	dirLight = new THREE.DirectionalLight(0xffffff, 2);
	dirLight.position.set(0, 20, 10);
	dirLight.castShadow = true;
	const d = 10;
	dirLight.shadow.camera.left = - d;
	dirLight.shadow.camera.right = d;
	dirLight.shadow.camera.top = d;
	dirLight.shadow.camera.bottom = - d;

	dirLight.shadow.camera.near = 2;
	dirLight.shadow.camera.far = 50;

	dirLight.shadow.mapSize.x = 1024;
	dirLight.shadow.mapSize.y = 1024;

	dirLight.shadow.bias = - 0.003;
	scene.add(dirLight);

	window.addEventListener('resize', onWindowResize);

	// stats
	stats = new Stats();
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
	const sphereGeometry = new THREE.SphereGeometry(sphere_radius - 0.1);
	const sphereMaterial = new THREE.MeshStandardMaterial({ color: 0xFFFFFF });
	sphereMaterial.roughness = 1.0;
	console.log(sphereMaterial);
	sphere = new THREE.Mesh(sphereGeometry, sphereMaterial);
	sphere.castShadow = true;
	sphere.position.set(0, 1, 1);
	scene.add(sphere);

	//cloth
	// Cloth graphic object
	const clothWidth = 4;
	const clothHeight = 3;
	const clothNumSegmentsZ = clothWidth * 5;
	const clothNumSegmentsY = clothHeight * 5;
	const clothPos = new THREE.Vector3(0, -1, 0);
	const clothMaterial = new THREE.MeshStandardMaterial({ 
		color: 0xFFFFFF, 
		side: THREE.DoubleSide,
		wireframe: false,
	 });
	const clothGeometry = new THREE.PlaneGeometry(clothWidth, clothHeight, clothNumSegmentsZ, clothNumSegmentsY);
	cloth = new THREE.Mesh(clothGeometry, clothMaterial);

	//clothGeometry.rotateX(Math.PI * 0.5);
	clothGeometry.rotateY(Math.PI);
	//clothGeometry.translate(0, 10.0, 0.0);
	//clothGeometry.translate(clothPos.x, clothPos.y + clothHeight * 0.5, clothPos.z - clothWidth * 0.5);
	cloth.castShadow = true;
	cloth.receiveShadow = true;
	cloth.position.set(0.0, 2.0, 0.0);
	scene.add(cloth);

	Module().then(function (module) 
	{
		PBD = module;
		PBDCloth = new PBD.ClothSim(clothWidth, clothHeight, clothNumSegmentsZ, clothNumSegmentsY);
		console.log(PBDCloth);
		PBDCloth.k = k;
		PBDCloth.damping = damping;
		PBDCloth.node_mass = mass;
		PBDCloth.gravity = gravity;
		// PBDCloth.dt = dt;
		PBDCloth.UpdateSphere(sphere.position.x,
			sphere.position.y,
			sphere.position.z,
			sphere_radius
			);
		const clothPositions = cloth.geometry.attributes.position.array;
		const numVerts = clothPositions.length / 3;
		var worldMatrix = cloth.worldMatrix;

		let indexFloat = 0;
		for (let i = 0; i < numVerts; i++, indexFloat+=3) 
		{
			var positionAttribute = new THREE.Vector3(
				clothPositions[indexFloat],
				clothPositions[indexFloat + 1],
				clothPositions[indexFloat + 2]);
			var x = (cloth.localToWorld(positionAttribute)).toArray();
			PBDCloth.SetPosition(i, x[0], x[1], x[2]);
		}
		cloth.geometry.computeVertexNormals();
		cloth.geometry.attributes.position.needsUpdate = true;
		cloth.geometry.attributes.normal.needsUpdate = true;
	});


	const albedo = new THREE.TextureLoader().load('./02Color.png', function (texture) 
	{
		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
	});
	const normal = new THREE.TextureLoader().load('./01Normal.png', function (texture) {
		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
	});
	const height = new THREE.TextureLoader().load('./01Displacement.png', function (texture) {
		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
	});
	const ao = new THREE.TextureLoader().load('./01AO.png', function (texture) {
		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
	});
	const roughness = new THREE.TextureLoader().load('./01Roughness.png', function (texture) {
		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
	});
	cloth.material.map = albedo;
	cloth.material.normalMap = normal;
	cloth.material.aoMap = ao;
	cloth.material.bumpMap = height;
	cloth.material.roughnessMap = roughness;

	const gui = new GUI();
	// gui.add(clothMaterial, 'wireframe').onChange(function(value)
	// {
	// 	if (!value)
	// 	{
	// 		cloth.material.map = albedo;
	// 		cloth.material.normalMap = normal;
	// 		cloth.material.aoMap = ao;
	// 		cloth.material.bumpMap = height;
	// 		cloth.material.roughnessMap = roughness;
	// 		cloth.material.needsUpdate = true;
	// 	}
	// 	else
	// 	{
	// 		cloth.material.map = null;
	// 		cloth.material.needsUpdate = true;
	// 	}

	// });
	gui.addColor(clothMaterial, 'color');
	gui.add(renderer.shadowMap, 'enabled');
	
	const controls = new OrbitControls(camera, renderer.domElement);
	controls.enablePan = false;
	controls.enableZoom = false;
	controls.target.set(0, 1, 0);
	controls.update();



	//gui.add(material, 'metalness', 0, 1);
	//gui.add(renderer, 'toneMappingExposure', 0, 2).name('exposure');

	//create_ui();
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




function animate() 
{
	requestAnimationFrame(animate);
	var deltaTime = clock.getDelta();

	//cloth.
	//sphere.position.z -= 0.01;

	renderer.render(scene, camera);
	if (PBDCloth)
	{
		//console.log(PBDCloth.Step(deltaTime));
		//
		updateCloth();
		PBDCloth.dt = deltaTime / 10.0;
		PBDCloth.Step(deltaTime);
		PBDCloth.UpdateSphere(
			sphere.position.x,
			sphere.position.y,
			sphere.position.z,
			sphere_radius
		);

		//console.log(PBDCloth.Print());
	}
	stats.update();
}

function updateCloth()
{
	const clothPositions = cloth.geometry.attributes.position.array;
	const numVerts = clothPositions.length / 3;

	let indexFloat = 0;
	for (let i = 0; i < numVerts; i++) 
	{
		var world_pos = new THREE.Vector3(
			PBDCloth.GetPositionX(i), 
			PBDCloth.GetPositionY(i), 
			PBDCloth.GetPositionZ(i));
		var x = cloth.worldToLocal(world_pos).toArray();
		clothPositions[indexFloat++] = x[0];
		clothPositions[indexFloat++] = x[1];
		clothPositions[indexFloat++] = x[2];

	}
	cloth.geometry.computeVertexNormals();
	cloth.geometry.attributes.position.needsUpdate = true;
	cloth.geometry.attributes.normal.needsUpdate = true;
}

function onMouseDown() {

}

function onMouseMove() {

}

function onMouseUp() {

}



