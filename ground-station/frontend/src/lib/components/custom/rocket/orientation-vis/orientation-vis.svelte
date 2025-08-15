<script lang="ts">
	import * as THREE from 'three';
	import { STLLoader } from 'three/examples/jsm/loaders/STLLoader.js';
	import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
	import * as Card from '@src/lib/components/ui/card';
	import { rocketData } from '@src/store';
	import { Button } from '@src/lib/components/ui/button';
	import gsap from 'gsap';

	let camera = new THREE.PerspectiveCamera();
	let renderer = new THREE.WebGLRenderer();
	let controls = new OrbitControls(camera, renderer.domElement);
	let mesh = new THREE.Mesh();
	let idealCameraPosition = new THREE.Vector3();

	let resetView = () => {
		camera.position.set(idealCameraPosition.x, idealCameraPosition.y, idealCameraPosition.z);
		controls.update();

		resetPosition();
	};

	let resetPosition = () => {
		gsap.to(mesh.rotation, {
			x: ($rocketData.orientation_x * Math.PI) / 180,
			y: ($rocketData.orientation_y * Math.PI) / 180,
			z: ($rocketData.orientation_z * Math.PI) / 180
		});
	};
	$effect.root(() => {
		const filePath = '/rocket.STL'; // Update with your STL file path
		let scene = new THREE.Scene();
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

					geometry.center();
					geometry.scale(0.5, 0.5, 0.5);
					mesh.position.set(0, 0, 0);
					scene.add(mesh);

					const axesHelper = new THREE.AxesHelper(500);
					scene.add(axesHelper);

					const cameraDistance = 1000; // Adjust distance to fit the mesh comfortably
					camera.position.set(cameraDistance, 0, cameraDistance);
					camera.getWorldPosition(idealCameraPosition);
					camera.lookAt(new THREE.Vector3(0, 0, 0));

					controls.target.copy(mesh.position);
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
				5000
			);

			controls = new OrbitControls(camera, renderer.domElement);

			scene.background = new THREE.Color(0x020719);

			const animate = () => {
				requestAnimationFrame(animate);
				renderer.render(scene, camera);
			};

			loadSTLFromPath();
			animate();
		}, 1000);
		$effect(() => resetPosition());
	});
</script>

<Card.Root class="relative h-[444px] w-[336px] bg-background p-2 text-gray-600">
	<canvas class="orientation-canvas aspect-square h-full w-full rounded-lg border"></canvas>
	<Button variant="default" class="absolute bottom-4 right-4" onclick={resetView}>refresh</Button>
</Card.Root>
