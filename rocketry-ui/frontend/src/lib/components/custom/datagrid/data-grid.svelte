<script lang="ts">
	import { activeTab, allDataEverything, allDataNames } from '@src/store';
	import SingleCellItem from '$lib/components/custom/cell-item/singleCellItem.svelte';
	import TwoCellItem from '$lib/components/custom/cell-item/twoCellItem.svelte';
	import FourCellItem from '$lib/components/custom/cell-item/fourCellItem.svelte';

	let gridDataNames: TtelemetryDataField[] = $state([]);

	$effect(() => {
		if ($activeTab == 'Overview') {
			gridDataNames = $allDataNames;
		} else if ($activeTab == 'Graphs') {
			gridDataNames = [
				'timeStamp',
				'altitude',
				'pressure',
				'temperature',
				'voltage',
				'gasConcn',
				'gnss_altitude',
				'velocity',
				'accel_x',
				'accel_y',
				'accel_z',
				'orientation_x',
				'orientation_y',
				'orientation_z'
			];
		}
	});
</script>

<ul class="datagrid grid w-full min-w-[1392px] grid-cols-4 gap-4 p-0">
	{#if $activeTab != 'Graphs'}
		<SingleCellItem datatype="teamID"></SingleCellItem>
		<SingleCellItem datatype="timeStamp"></SingleCellItem>
		<SingleCellItem datatype="packetCount"></SingleCellItem>
		<SingleCellItem datatype="softwareState"></SingleCellItem>
	{/if}
	<SingleCellItem datatype="altitude"></SingleCellItem>
	<SingleCellItem datatype="pressure"></SingleCellItem>
	<SingleCellItem datatype="temperature"></SingleCellItem>
	<SingleCellItem datatype="voltage"></SingleCellItem>
	{#if $activeTab == 'Overview'}
		<FourCellItem></FourCellItem>
	{/if}
	<TwoCellItem datatype="orientation"></TwoCellItem>
	<TwoCellItem datatype="accel"></TwoCellItem>
	<SingleCellItem datatype="gasConcn"></SingleCellItem>
	<SingleCellItem datatype="velocity"></SingleCellItem>
</ul>
