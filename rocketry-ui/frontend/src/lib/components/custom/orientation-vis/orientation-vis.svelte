<script lang="ts">
	import * as THREE from 'three';
	import { STLLoader } from 'three/examples/jsm/loaders/STLLoader.js';
	import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
	import * as Card from '@src/lib/components/ui/card';
	import { allGraphPastDatas, readData } from '@src/store';
	import { Button } from '../../ui/button';
	import { Easing, Tween } from '@tweenjs/tween.js';

	const filePath = 'static/rocket.STL'; // Update with your STL file path
	let scene = new THREE.Scene();
	let camera = new THREE.PerspectiveCamera();
	let renderer = new THREE.WebGLRenderer();
	let controls = new OrbitControls(camera, renderer.domElement);
	let mesh = new THREE.Mesh();
	let idealCameraPosition = new THREE.Vector3();

	let rotateScene = () => {
		$effect(() => {
			resetPosition();
		});
	};

	let resetView = () => {
		camera.position.set(idealCameraPosition.x, idealCameraPosition.y, idealCameraPosition.z);
		controls.update();

		resetPosition();
	};

	let resetPosition = () => {
		mesh.setRotationFromAxisAngle(
			new THREE.Vector3(1, 0, 0),
			($allGraphPastDatas.orientation_x[0] * Math.PI) / 180
		);
		mesh.setRotationFromAxisAngle(
			new THREE.Vector3(0, 1, 0),
			($allGraphPastDatas.orientation_y[0] * Math.PI) / 180
		);
		mesh.setRotationFromAxisAngle(
			new THREE.Vector3(0, 0, 1),
			($allGraphPastDatas.orientation_z[0] * Math.PI) / 180
		);
	};

	$effect(() => {
		setTimeout(() => {
			const loadSTLFromPath = async () => {
				try {
					const response = await fetch(filePath);

					if (!response.ok) {
						throw new Error(`not ok: ${response.statusText}`);
					}
					const arrayBuffer = await response.arrayBuffer();
					const loader = new STLLoader();
					const geometry = loader.parse(arrayBuffer);
					getSize(geometry);

					const material = new THREE.MeshMatcapMaterial({
						color: 0xffffff
					});

					geometry.computeBoundingBox();
					const boundingBox = geometry.boundingBox || new THREE.Box3();
					const center = new THREE.Vector3();
					boundingBox.getCenter(center);

					mesh = new THREE.Mesh(geometry, material);
					mesh.name = 'rocket mesh';

					mesh.position.set(0, 0, 0);
					geometry.center();
					scene.add(mesh);

					mesh.setRotationFromAxisAngle(
						new THREE.Vector3(1, 0, 0),
						-($allGraphPastDatas.orientation_x[0] || 0 * Math.PI) / 180
					);
					mesh.setRotationFromAxisAngle(
						new THREE.Vector3(0, 1, 0),
						-($allGraphPastDatas.orientation_y[0] || 0 * Math.PI) / 180
					);
					mesh.setRotationFromAxisAngle(
						new THREE.Vector3(0, 0, 1),
						-($allGraphPastDatas.orientation_z[0] || 0 * Math.PI) / 180
					);

					const axesHelper = new THREE.AxesHelper(1000);
					scene.add(axesHelper);

					const cameraDistance = center.y * 2; // Adjust distance to fit the mesh comfortably
					camera.position.set(cameraDistance, center.y, cameraDistance);
					camera.getWorldPosition(idealCameraPosition);
					camera.lookAt(new THREE.Vector3(0, 0, 0));
					scene.translateY(center.y);

					controls.target.copy(center);
					controls.update();
				} catch (error) {
					console.error('Error loading STL file:', error);
				}
			};

			function getSize(geometry: THREE.BufferGeometry<THREE.NormalBufferAttributes>) {
				let size = new THREE.Vector3();
				const boundingBox = geometry.boundingBox || new THREE.Box3();
				geometry.computeBoundingBox();
				boundingBox.getSize(size);
			}

			const canvas = document.querySelector('.orientation-canvas');
			if (!canvas) {
				console.error('Canvas element not found');
				return;
			}

			renderer = new THREE.WebGLRenderer({ canvas });
			renderer.setSize(
				canvas.getBoundingClientRect().width,
				canvas.getBoundingClientRect().height,
				false
			);

			camera = new THREE.PerspectiveCamera(
				69,
				canvas.getBoundingClientRect().width / canvas.getBoundingClientRect().height,
				0.1,
				9999
			);

			controls = new OrbitControls(camera, renderer.domElement);
			controls.rotateSpeed = 0.1;

			scene.background = new THREE.Color(0x020719);

			const resizeCanvasToDisplaySize = () => {
				const width = canvas.getBoundingClientRect().width;
				const height = canvas.getBoundingClientRect().height;
				camera.aspect = width / height;
				camera.updateProjectionMatrix();
			};

			const animate = () => {
				requestAnimationFrame(animate);
				renderer.render(scene, camera);
			};

			loadSTLFromPath();
			animate();
		}, 1000);
		rotateScene();
	});
</script>

<Card.Root class="bg-background relative h-[444px] w-[336px] p-2 text-gray-600">
	<canvas class="orientation-canvas aspect-square h-full w-full rounded-lg border"></canvas>
	<Button variant="default" class="absolute bottom-4 right-4" onclick={resetView}>refresh</Button>
</Card.Root>
