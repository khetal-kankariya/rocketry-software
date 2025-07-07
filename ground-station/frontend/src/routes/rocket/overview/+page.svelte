<script lang="ts">
	import TelemetryData from '$lib/components/ui/telemetry-data/telemetry-data.svelte';
	import SensorStatus from '@src/lib/components/custom/rocket/sensor-status/sensor-status.svelte';
	import OrientationVis from '@src/lib/components/custom/rocket/orientation-vis/orientation-vis.svelte';
	import DataGrid from '@src/lib/components/custom/rocket/datagrid/data-grid.svelte';
	import { activeTab, GCS_started, rocketData } from '@src/store';
	import Map from '@src/lib/components/custom/rocket/map/map.svelte';
	import { onMount } from 'svelte';
	import { get_startupdata, save_startupdata } from '@src/socket-interface';

	// $effect.root(() => {
	// 	import('@src/dataupdater');
	// 	get_startupdata();
	// 	save_startupdata();
	// 	import('@src/socket-interface');

	// 	if (window) {
	// 		let latest_data = JSON.parse(window.sessionStorage.getItem('latest-data') || '{}');
	// 		if (Object.keys(latest_data).length > 0) {
	// 			rocketData.set(latest_data);
	// 		}
	// 	}
	// });

	$effect(() => {
		$activeTab = 'Overview';
		$GCS_started = true;
	});
</script>

<TelemetryData></TelemetryData>
<DataGrid></DataGrid>
<div class="flex gap-4">
	<OrientationVis></OrientationVis>
	<Map></Map>
	<SensorStatus></SensorStatus>
</div>

<style lang="scss">
</style>
