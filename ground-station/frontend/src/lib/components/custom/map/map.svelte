<script lang="ts">
	import 'leaflet/dist/leaflet.css';
	import { sim_coords } from '@src/simulationData';
	import * as Card from '$lib/components/ui/card';
	import { onMount } from 'svelte';
	import type { LatLngExpression } from 'leaflet';
	import { simulation_mode, travelled_coords } from '@src/store';

	let L: typeof import('leaflet');
	var map: L.Map;
	let mapEl: HTMLElement;
	let markers: L.Marker[] = [];
	let local_coords = $derived($simulation_mode ? $sim_coords : $travelled_coords);
	let mapcenter: L.LatLngExpression = $derived.by(() => {
		let sum: L.LatLngExpression = [0, 0];
		local_coords.forEach((coord) => {
			sum[0] += (coord as number[])[0];
			sum[1] += (coord as number[])[1];
		});
		sum[0] /= local_coords.length;
		sum[1] /= local_coords.length;

		return sum;
	});

	function addTileLayer(map: L.Map) {
		L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
			maxZoom: 20,
			subdomains: ['mt0', 'mt1', 'mt2', 'mt3']
		}).addTo(map);
	}

	function addMarker(map: L.Map, coord: LatLngExpression) {
		let new_marker: L.Marker = L.marker(coord);
		new_marker.addTo(map);
		markers.unshift(new_marker);
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
		(function () {
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
			return control;
		})().addTo(map);
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

	onMount(() => {
		(async () => {
			L = await import('leaflet');
			map = L.map(mapEl, {
				attributionControl: false,
				center: mapcenter,
				zoomControl: false,
				markerZoomAnimation: true
			});

			addTileLayer(map);

			setControls(map);

			addResetbtn(map);

			addPolygon(map);

			fitBounds(map);
		})();
	});

	travelled_coords.subscribe((val) => {
		let new_coord = val[val.length - 1];
		// addMarker(map, new_coord);
		// removeMarker(markers[0]);
	});
</script>

<Card.Root class="h-[444px] w-[688px] p-2">
	<div class="h-full w-full rounded-md" bind:this={mapEl} id="mapEl"></div>
</Card.Root>
