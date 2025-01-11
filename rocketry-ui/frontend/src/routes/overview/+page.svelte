<script lang="ts">
	import TelemetryData from '$lib/components/ui/telemetry-data/telemetry-data.svelte';
	import SensorStatus from '@src/lib/components/custom/sensor-status/sensor-status.svelte';
	import OrientationVis from '@src/lib/components/custom/orientation-vis/orientation-vis.svelte';
	import DataGrid from '@src/lib/components/custom/datagrid/data-grid.svelte';
	import { activeTab } from '@src/store';
	import Map from '@src/lib/components/custom/map/map.svelte';

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
	::-webkit-scrollbar {
		display: none;
	}
</style>
