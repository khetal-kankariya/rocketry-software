<script lang="ts">
	import 'leaflet/dist/leaflet.css';
	import { sim_coords } from '@src/simulationData';
	import * as Card from '$lib/components/ui/card';
	import { onMount, untrack } from 'svelte';
	import type { LatLngBounds, LatLngExpression } from 'leaflet';
	import { simulation_mode, travelled_coords } from '@src/store';
	import L from 'leaflet';
	import type { Action } from 'svelte/action';

	var map: L.Map;
	let markers: L.Marker[] = [];
	const default_coord = [22, 72];
	let local_coords = $derived($travelled_coords.length ? $travelled_coords : [default_coord]);

	let mapaction: Action = (node) => {
		map = L.map(node, {
			attributionControl: false,
			center: [22, 72],
			zoomControl: false,
			zoom: 10
		});
		addTileLayer(map);
		setControls(map);
		addResetbtn(map);

		$effect(() => {
			if (local_coords.length) {
				addMarker(map);
				fitBounds(map);
			}
		});
	};

	function addTileLayer(map: L.Map) {
		L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
			// maxZoom: 25,
			subdomains: ['mt0', 'mt1', 'mt2', 'mt3']
		}).addTo(map);
	}

	function addMarker(map: L.Map) {
		const lastCoord = local_coords.at(-1);
		if (lastCoord !== undefined) {
			let new_marker = L.marker(lastCoord);
			new_marker.addTo(map);
			markers.unshift(new_marker);
		}
	}

	function removeMarker(marker: L.Marker) {
		marker.remove();
	}

	function fitBounds(map: L.Map) {
		var bounds = L.latLngBounds(local_coords);
		map.fitBounds(bounds, {
			padding: [40, 40]
		});
	}

	function addResetbtn(map: L.Map) {
		var control = new L.Control({ position: 'topright' });
		control.onAdd = function (map) {
			var resetBTN = L.DomUtil.create('a', 'resetzoom');
			resetBTN.innerHTML = 'Reset Zoom';
			resetBTN.style.background = '#fff';
			resetBTN.style.padding = '4px 8px';
			resetBTN.style.borderRadius = '8px';
			resetBTN.style.color = '#000';
			resetBTN.style.cursor = 'pointer';
			L.DomEvent.disableClickPropagation(resetBTN).addListener(
				resetBTN,
				'click',
				function () {
					fitBounds(map);
				},
				resetBTN
			);
			return resetBTN;
		};
		control.addTo(map);
	}

	function setControls(map: L.Map) {
		L.control
			.scale({
				metric: true,
				imperial: false,
				position: 'topleft'
			})
			.addTo(map);
	}

	function addPolygon(map: L.Map) {
		L.polyline(local_coords, {
			color: 'red',
			fillOpacity: 0
		}).addTo(map);
	}

	travelled_coords.subscribe((new_coords) => {
		const lastCoord = new_coords.at(-1);
		if (lastCoord !== undefined) local_coords.push(lastCoord);
	});
</script>

<Card.Root class="h-[444px] w-[688px] p-2">
	<div class="h-full w-full rounded-md" use:mapaction id="mapEl"></div>
</Card.Root>
