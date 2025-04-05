import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';

//import Stats from 'three/addons/libs/stats.module.js'

const RotationDirection = -180, // Set any number of degrees, negative for "anti-spin" and posive for "pro-spin"
	// NOTE: Anything that isn't divisible by 180 will have a funky jerking motion when switching planes
	// TODO: Need to figure out how to ease between them

	ttotal = 1.5, // How long each animation plays for
	play = true, // Disables animating (displays all of the paths / planes)
	radius = 5, // Radius of points from center
	offset = new THREE.Vector3(0, 0, 0); // offset from center (Haven't tested, in the future multiple "props" will be animated)

// The following is so that I can quickly copy over to JSFIDDLE.NET, as this is a Vue Component
let pobj, prect, width, height;
try {
	pobj = domParent.value;
	prect = pobj.getBoundingClientRect();
	(width = prect.width), (height = prect.height);
} catch {
	pobj = document.body;
	width = window.innerWidth;
	height = window.innerHeight;
}

const sets = [], // For debugging specific points
	//sets = [['MTC', 'BC'], ['MTL', 'BC']],
	//sets = [['MTC','BTC']],

	renderer = new THREE.WebGLRenderer({ antialias: true }),
	scene = new THREE.Scene(),
	camera = new THREE.PerspectiveCamera( // Alternate camera: Orthographic
		45, // fov —    field of view
		width / height, // aspect — aspect ratio
		0.1, // near —   near plane
		1000 // far —    far plane
	),
	orbit = new OrbitControls(camera, renderer.domElement),
	dispose = [],
	multi = radius / 5; // Adjustment for changes in Radius (5 being the original)

renderer.setPixelRatio(window.devicePixelRatio);
renderer.setSize(width, height);
renderer.setClearColor(0x444444);
pobj.appendChild(renderer.domElement);

// Items we must call dispose() on in order to free up memory
dispose.push(renderer, orbit);

//camera.position.set(-3.6648487120891953 * multi, 9.052174822127743 * multi, -13.214651524241402 * multi) // XYZ
camera.position.set(0, 0, -14 * multi); // XYZ
orbit.update(); // Must be called whenever camera position is updated

const pcolor = [0xffff00, 0x00ffff, 0xff00ff, 0xffa500, 0x00ff00],
	//pcolor = [0x00FF00, 0x0000FF, 0xFF0000, 0xFFFFFF],

	//psize  = [0.4,      0.3,      0.26,     0.26],
	psize = [0.06, 0.06, 0.06, 0.06],
	//psize  = [0.2,      0.2,      0.2,      0.2],

	pOct = 0, // "Octahedral" Points (6 total, but forms 8 sides)
	pBox = 1, // "Box Mode" Points
	pOut = 2, // Corner Points
	pMid = 3, // Middle Point
	// First item is the type of point, the rest are Cartesian coords to transform into a sphere
	points = {
		// Front 9 Points
		FTL: [pOut, radius, radius, radius], // Front Top Left
		FTC: [pBox, 0, radius, radius], // Front Top
		FTR: [pOut, -radius, radius, radius], // Front Top Right
		FL: [pBox, radius, 0, radius], // Front Left
		FC: [pOct, 0, 0, radius], // Front
		FR: [pBox, -radius, 0, radius], // Front Right
		FBL: [pOut, radius, -radius, radius], // Front Bottom Left
		FBC: [pBox, 0, -radius, radius], // Front Bottom Center
		FBR: [pOut, -radius, -radius, radius], // Front Bottom Right
		// Middle 9 Points
		MTL: [pBox, radius, radius, 0], // Middle Top Left
		MTC: [pOct, 0, radius, 0], // Middle Top
		MTR: [pBox, -radius, radius, 0], // Middle Top Right
		ML: [pOct, radius, 0, 0], // Middle Left
		MC: [pMid, 0, 0, 0], // Middle
		MR: [pOct, -radius, 0, 0], // Middle Right
		MBL: [pBox, radius, -radius, 0], // Middle Bottom Left
		MBC: [pOct, 0, -radius, 0], // Middle Bottom
		MBR: [pBox, -radius, -radius, 0], // Middle Bottom Right
		// Back 9 Points
		BTL: [pOut, radius, radius, -radius], // Back Top Left
		BTC: [pBox, 0, radius, -radius], // Back Top
		BTR: [pOut, -radius, radius, -radius], // Back Top Right
		BL: [pBox, radius, 0, -radius], // Back Left
		BC: [pOct, 0, 0, -radius], // Back
		BR: [pBox, -radius, 0, -radius], // Back Right
		BBL: [pOut, radius, -radius, -radius], // Back Bottom Left
		BBC: [pBox, 0, -radius, -radius], // Back Bottom
		BBR: [pOut, -radius, -radius, -radius] // Back Bottom Right
	},
	pobjs = {},
	pdeg = {},
	show = {},
	axis1 = new THREE.Vector3(1, 0, 0),
	axis2 = new THREE.Vector3(0, 1, 0),
	a1x = new THREE.Quaternion(),
	a1y = new THREE.Quaternion();

// Show or hide specific types points
show[pOct] = true;
show[pBox] = true;
show[pOut] = true;
show[pMid] = true;

// Builds the Points
let sph1 = new THREE.Spherical();
for (let key in points) {
	const p = points[key],
		sphere = new THREE.Mesh(
			new THREE.SphereGeometry(psize[p[0]] * multi, 20, 20),
			new THREE.MeshBasicMaterial({ color: pcolor[p[0]] })
		);

	pobjs[key] = sphere;
	if (key == 'MC') {
		// Middle point
		sphere.position.add(offset);
	} else {
		sph1.setFromCartesianCoords(p[1], p[2], p[3]);
		pdeg[key] = [sph1.phi, sph1.theta];
		sphere.position.setFromSphericalCoords(radius, sph1.phi, sph1.theta).add(offset);
	}
	if (show[p[0]]) {
		scene.add(sphere);
	}
}

const guides = [
	// Each array is in a specific order for navigating the plane
	['MBC', 'MBR', 'MR', 'MTR', 'MTC', 'MTL', 'ML', 'MBL'], // Wall
	['MTC', 'FTC', 'FC', 'FBC', 'MBC', 'BBC', 'BC', 'BTC'], // Wheel
	['FC', 'FR', 'MR', 'BR', 'BC', 'BL', 'ML', 'FL'], // Horizontal

	['MTC', 'FTR', 'FR', 'FBR', 'MBC', 'BBL', 'BL', 'BTL'],
	['MTC', 'BTR', 'BR', 'BBR', 'MBC', 'FBL', 'FL', 'FTL'],
	['FTC', 'FTR', 'MR', 'BBR', 'BBC', 'BBL', 'ML', 'FTL'],
	['BTC', 'BTR', 'MR', 'FBR', 'FBC', 'FBL', 'ML', 'BTL'],
	['MTL', 'FTL', 'FC', 'FBR', 'MBR', 'BBR', 'BC', 'BTL'],
	['MTR', 'BTR', 'BC', 'BBL', 'MBL', 'FBL', 'FC', 'FTR'],
	['FTC', 'FR', 'MBR', 'BBC', 'BL', 'MTL'],
	['BTC', 'BL', 'MBL', 'FBC', 'FR', 'MTR'],
	['MTR', 'BR', 'BBC', 'MBL', 'FL', 'FTC'],
	['MTL', 'FL', 'FBC', 'MBR', 'BR', 'BTC'],
	['BTL', 'FL', 'FBR', 'BR'],
	['FTR', 'BR', 'BBL', 'FL'],
	['BTR', 'BL', 'FBL', 'FR'],
	['FTL', 'FR', 'BBR', 'BL'],
	['FTC', 'FBR', 'BBC', 'BTL'],
	['BTL', 'MBL', 'FBR', 'MTR'],
	['BTR', 'BBC', 'FBL', 'FTC'],
	['MTL', 'FBL', 'MBR', 'BTR'],
	['FTL', 'FBC', 'BBR', 'BTC'],
	['BTC', 'BBL', 'FBC', 'FTR'],
	['MTR', 'BBR', 'MBL', 'FTL'],
	['FTR', 'MBR', 'BBL', 'MTL']
],
	gsel = -1, // Set a specific guide as RED (index of the above) - Set to -1 for none
	guideMat = new THREE.MeshBasicMaterial({ color: 0xaaaaaa }),
	guideGeo = new THREE.TorusGeometry(radius, 0.01 * multi, 10, 100),
	norm = new THREE.Vector3(),
	vec0 = new THREE.Vector3(),
	matrix = new THREE.Matrix4(),
	gobjs = [],
	gref = {};

for (let i = 0; i < guides.length; i++) {
	const objs = guides[i],
		guide = new THREE.Mesh(
			guideGeo,
			i != gsel ? guideMat : new THREE.MeshBasicMaterial({ color: 0xff0000 })
		),
		// First and opposing points for calculating triangle based rotation
		triangle = new THREE.Triangle(
			pobjs[objs[0]].position,
			pobjs[objs[objs.length / 2 - 1]].position, // Two furthest points which aren't the opposing side
			pobjs[objs[objs.length / 2 + 1]].position // this creates a triangle for us to calculate with
		);
	// Determine the rotation for the plane based on three points
	triangle.getNormal(norm);
	matrix.lookAt(norm, vec0, vec0);
	const euler = new THREE.Euler().setFromRotationMatrix(matrix);
	guide.position.add(offset);
	guide.rotation.set(euler.x, euler.y, euler.z);
	guide.visible = true;
	scene.add(guide);

	// Structures for accessing suffz!
	gobjs.push(guide);
	for (let j = 0; j < objs.length; j++) {
		const key = objs[j];
		if (gref[key] === undefined) {
			gref[key] = {};
		}
		for (let k = 0; k < objs.length; k++) {
			const key2 = objs[k];
			// Skip the same and opposing
			if (key == key2 || pobjs[key].position.distanceTo(pobjs[key2].position) > radius * 2 * 0.99) {
				continue;
			}
			if (gref[key][key2] !== undefined) {
				console.log('OVERWRITE: ', key, objs[k]);
			}
			gref[key][key2] = i;
		}
	}
}

// --- Stuff to remove later

//const axesHelper = new THREE.AxesHelper(radius)
//scene.add(axesHelper)
//dispose.push(axesHelper)

// ---

const model = new THREE.Mesh(
	new THREE.CylinderGeometry(0.1, 0.1, 3.5, 32),
	new THREE.MeshBasicMaterial({ color: 0x00ff00 })
);
model.visible = play;
scene.add(model);

// Random points to animate from
const pkeys = Object.keys(pobjs);
// Strip the center point out
for (let a = 0; a < pkeys.length; a++) {
	if (pkeys[a] == 'MC') {
		pkeys.splice(a, 1);
		//a--
		break;
	}
}

// Vars to use in the below animation
let tstart = -1,
	o1,
	o2 = pkeys[Math.floor(pkeys.length * Math.random())],
	obj1,
	obj2 = pobjs[o2],
	pnt1,
	pnt2 = points[o2],
	cset = 0;

let sphericalStart = new THREE.Spherical(),
	sphericalEnd = new THREE.Spherical(radius, pdeg[o2][0], pdeg[o2][1]),
	vectorStart = new THREE.Vector3(),
	vectorEnd = new THREE.Vector3(),
	angleEnd,
	normal = new THREE.Vector3(),
	rot = new THREE.Vector3(),
	rotation = {
		perform: RotationDirection, // --- NOTE: This is what tells it how much to spin, and in which direction!
		current: 0 // Tracks orientation
	};

const reduceDeg = (deg) => {
	let ret = deg % 360;
	return ret;
},
	animate = (time) => {
		if (play) {
			const keys = Object.keys(gref[o2]);

			if (tstart < 0 || time > tstart + ttotal * 1000) {
				rotation.current = reduceDeg(reduceDeg(rotation.current) + reduceDeg(rotation.perform));

				// console.log( rotation.current )

				tstart = time;
				o1 = o2;
				o2 = keys[Math.floor(Math.random() * keys.length)];
				obj1 = obj2;
				obj2 = pobjs[o2];
				pnt1 = pnt2;
				pnt2 = points[o2];

				if (sets.length) {
					o1 = sets[cset][0];
					o2 = sets[cset][1];
					obj1 = pobjs[o1];
					obj2 = pobjs[o2];
					pnt1 = points[o1];
					pnt2 = points[o2];

					cset++;
					if (cset == sets.length) {
						cset = 0;
					}
				}

				//console.log( o1, o2 )

				// Calculate spherical path for both position and rotation
				sphericalStart.set(radius, pdeg[o1][0], pdeg[o1][1]);
				sphericalEnd.set(radius, pdeg[o2][0], pdeg[o2][1]);
				vectorStart.setFromSpherical(sphericalStart);
				vectorEnd.setFromSpherical(sphericalEnd);
				normal.copy(vectorStart).cross(vectorEnd).normalize();
				angleEnd = vectorStart.angleTo(vectorEnd);

				// Show the plane we are currently animating on
				const gi = gref[o1][o2];
				for (let i = 0; i < gobjs.length; i++) {
					gobjs[i].visible = i == gi ? true : false;
				}
			}

			// Percentage completed of the animation
			const perc = (time - tstart) / (ttotal * 1000);

			// Animates between points
			model.position
				.copy(vectorStart)
				.applyAxisAngle(normal, perc * angleEnd)
				.add(offset);

			// This is where the rotation magic happens
			rot
				.copy(vectorStart)
				.applyAxisAngle(
					normal,
					THREE.MathUtils.degToRad(rotation.current) +
					perc * angleEnd +
					THREE.MathUtils.degToRad(rotation.perform) * perc
				)
				.add(offset);

			// Convert position to rotation
			sph1.setFromCartesianCoords(rot.x - offset.x, rot.y - offset.y, rot.z - offset.z);
			a1x.setFromAxisAngle(axis1, sph1.phi);
			a1y.setFromAxisAngle(axis2, sph1.theta);
			model.quaternion.copy(a1x).premultiply(a1y);
		}

		renderer.render(scene, camera);
		if (stats) {
			stats.update();
		}
	},
	resize = () => {
		try {
			pobj = domParent.value;
			prect = pobj.getBoundingClientRect();
			(width = prect.width), (height = prect.height);
		} catch {
			pobj = document.body;
			width = window.innerWidth;
			height = window.innerHeight;
		}
		camera.aspect = width / height;
		camera.updateProjectionMatrix();
		renderer.setSize(width, height);
	};

renderer.setAnimationLoop(animate);

let stats;
try {
	stats = new Stats();
	document.body.appendChild(stats.dom);
} catch (error) { }

window.addEventListener('resize', resize);

// Built the Guides data with this
let ilist = [];
const raycaster = new THREE.Raycaster();
const pointer = new THREE.Vector2(),
	domClick = (e) => {
		pointer.x = (e.clientX / window.innerWidth) * 2 - 1;
		pointer.y = -(e.clientY / window.innerHeight) * 2 + 1;
		// update the picking ray with the camera and pointer position
		raycaster.setFromCamera(pointer, camera);
		// calculate objects intersecting the picking ray
		const intersects = raycaster.intersectObjects(scene.children);
		toploop: for (let i = 0; i < intersects.length; i++) {
			let obj = intersects[i].object;
			for (let key in pobjs) {
				if (pobjs[key].id == obj.id) {
					if (key == 'MC') {
						console.log("'" + ilist.join("', '") + "'");
						ilist = [];
					} else {
						ilist.push(key);
					}
					console.log("CLICKED: '" + key + "'");
					//alert('Clicked ' + key)
					break toploop;
				}
			}
		}
	};

renderer.domElement.addEventListener('click', domClick);

// Gonna need to research / debug this further (not working well) but it sort of works
// And I want to click objects in the future UI for selecting points, etc. that goes for Mobile as well!
renderer.domElement.addEventListener('touchend', domClick);

// JSFIDDLE Note: This is a Vue Component, and below code was removed. Ex: onUnmounted to dispose, etc.
