<script lang="ts">
	import TelemetryData from '$lib/components/ui/telemetry-data/telemetry-data.svelte';
	import SensorStatus from '@src/lib/components/custom/payload/sensor-status/sensor-status.svelte';
	import OrientationVis from '@src/lib/components/custom/payload/orientation-vis/orientation-vis.svelte';
	import DataGrid from '@src/lib/components/custom/payload/datagrid/data-grid.svelte';
	import { activeTab, GCS_started } from '@src/store';

	$effect(() => {
		$activeTab = 'Overview';

		$GCS_started = true;
	});
</script>

<TelemetryData></TelemetryData>
<DataGrid></DataGrid>
<div class="flex gap-4">
	<OrientationVis></OrientationVis>
	<SensorStatus></SensorStatus>
</div>

<style lang="scss">
</style>
