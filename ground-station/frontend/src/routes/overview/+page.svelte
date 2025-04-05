<script lang="ts">
	import TelemetryData from '$lib/components/ui/telemetry-data/telemetry-data.svelte';
	import SensorStatus from '@src/lib/components/custom/sensor-status/sensor-status.svelte';
	import OrientationVis from '@src/lib/components/custom/orientation-vis/orientation-vis.svelte';
	import DataGrid from '@src/lib/components/custom/datagrid/data-grid.svelte';
	import { activeTab, readData } from '@src/store';
	import Map from '@src/lib/components/custom/map/map.svelte';
	import { onMount } from 'svelte';
	import { get_startupdata, save_startupdata } from '@src/socket-interface';

	onMount(() => {
		import('@src/dataupdater');
		get_startupdata();
		save_startupdata();
		import('@src/socket-interface');

		if (window) {
			let latest_data = JSON.parse(window.sessionStorage.getItem('latest-data') || '{}');
			if (Object.keys(latest_data).length > 0) {
				readData.set(latest_data);
			}
		}
	});

	$effect(() => {
		$activeTab = 'Overview';
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
