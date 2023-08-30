import './style/main.css';
import * as THREE from 'three';

import { GUI } from 'three/examples/jsm/libs/lil-gui.module.min.js';
import Stats from 'three/examples/jsm/libs/stats.module.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';




const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

const geometry = new THREE.BoxGeometry(1, 1, 1);
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const cube = new THREE.Mesh(geometry, material);
scene.add(cube);

camera.position.z = 10;

// The cloth
// Cloth graphic object
const clothWidth = 4;
const clothHeight = 3;
const clothNumSegmentsZ = clothWidth * 5;
const clothNumSegmentsY = clothHeight * 5;
const clothPos = new THREE.Vector3(0, 0, 0);

const clothGeometry = new THREE.PlaneGeometry(clothWidth, clothHeight, clothNumSegmentsZ, clothNumSegmentsY);
clothGeometry.rotateY(Math.PI * 0.5);
clothGeometry.translate(clothPos.x, clothPos.y + clothHeight * 0.5, clothPos.z - clothWidth * 0.5);
let cloth;



const clothMaterial = new THREE.MeshLambertMaterial({ color: 0xFFFFFF, side: THREE.DoubleSide });
cloth = new THREE.Mesh(clothGeometry, clothMaterial);
cloth.castShadow = true;
cloth.receiveShadow = true;
scene.add(cloth);
new THREE.TextureLoader().load('./images.jpg', function (texture) {

	texture.colorSpace = THREE.SRGBColorSpace;
	texture.wrapS = THREE.RepeatWrapping;
	texture.wrapT = THREE.RepeatWrapping;
	texture.repeat.set(clothNumSegmentsZ, clothNumSegmentsY);
	cloth.material.map = texture;
	cloth.material.needsUpdate = true;

});

function animate() {
	requestAnimationFrame(animate);

	cube.rotation.x += 0.01;
	cube.rotation.y += 0.01;

	renderer.render(scene, camera);
}

// console.log(PBD._test_malloc(1, 2));




animate();