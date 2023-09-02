import './style/main.css';
import * as THREE from 'three';

import { GUI } from 'three/examples/jsm/libs/lil-gui.module.min.js';
import Stats from 'three/examples/jsm/libs/stats.module.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import Module from './PBD.js';




let PBD;
let scene, renderer, camera, stats, container;
let cloth, PBDCloth;
var clock = new THREE.Clock();
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
	const clothMaterial = new THREE.MeshStandardMaterial({ 
		color: 0xFFFFFF, 
		side: THREE.DoubleSide,
		wireframe: true,
	 });
	const clothGeometry = new THREE.PlaneGeometry(clothWidth, clothHeight, clothNumSegmentsZ, clothNumSegmentsY);
	cloth = new THREE.Mesh(clothGeometry, clothMaterial);

	clothGeometry.rotateX(Math.PI * 0.5);
	//clothGeometry.translate(0, 10.0, 0.0);
	//clothGeometry.translate(clothPos.x, clothPos.y + clothHeight * 0.5, clothPos.z - clothWidth * 0.5);
	cloth.castShadow = true;
	cloth.receiveShadow = true;
	cloth.position.set(0.0, 2.0, 0.0);
	scene.add(cloth);

	Module().then(function (module) 
	{
		PBD = module;
		PBDCloth = new PBD.ClothSim(clothNumSegmentsZ, clothNumSegmentsY);
		console.log(PBDCloth);
		const clothPositions = cloth.geometry.attributes.position.array;
		const numVerts = clothPositions.length / 3;

		let indexFloat = 0;
		for (let i = 0; i < numVerts; i++, indexFloat+=3) 
		{
			PBDCloth.SetPosition(i, 
				clothPositions[indexFloat],
				clothPositions[indexFloat + 1],
				clothPositions[indexFloat + 2]);
		}
		cloth.geometry.computeVertexNormals();
		cloth.geometry.attributes.position.needsUpdate = true;
		cloth.geometry.attributes.normal.needsUpdate = true;
	});


	const texture = new THREE.TextureLoader().load('./images.jpg', function (texture) 
	{
		texture.colorSpace = THREE.SRGBColorSpace;
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(1, 1);
	});
	const gui = new GUI();
	gui.add(clothMaterial, 'wireframe').onChange(function(value)
	{
		if (!value)
		{
			cloth.material.map = texture;
			cloth.material.needsUpdate = true;
		}
		else
		{
			cloth.material.map = null;
			cloth.material.needsUpdate = true;
		}

	});
	gui.addColor(clothMaterial, 'color');
	gui.add(renderer.shadowMap, 'enabled');



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

	//cloth.rotation.x += 0.01;

	renderer.render(scene, camera);
	if (PBDCloth)
	{
		//console.log(PBDCloth.Step(deltaTime));
		//
		updateCloth();
		console.log(PBDCloth.Step());
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
		clothPositions[indexFloat++] = PBDCloth.GetPositionX(i, 0);
		clothPositions[indexFloat++] = PBDCloth.GetPositionY(i, 1);
		clothPositions[indexFloat++] = PBDCloth.GetPositionZ(i, 2);

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



