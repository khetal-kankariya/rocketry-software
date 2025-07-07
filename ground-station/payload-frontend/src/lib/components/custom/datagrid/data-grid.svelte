<script lang="ts">
	import { activeTab, payloadDataInfo, payloadAllDataNames } from '@src/store';
	import SingleCellItem from '@src/lib/components/custom/cell-item/one-cell.svelte';
	import TwoCellItem from '@src/lib/components/custom/cell-item/two-cell.svelte';

	let gridDataNames: TdataFields[] = $state([]);

	$effect(() => {
		if ($activeTab == 'Overview') {
			gridDataNames = $payloadAllDataNames;
		} else if ($activeTab == 'Graphs') {
			gridDataNames = [
				'altitude',
				'pressure',
				'temperature',
				'gasConcn',
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
		<SingleCellItem datatype="packetCount"></SingleCellItem>
		<SingleCellItem datatype="softwareState"></SingleCellItem>
	{/if}
	<SingleCellItem datatype="altitude"></SingleCellItem>
	<SingleCellItem datatype="pressure"></SingleCellItem>
	<SingleCellItem datatype="temperature"></SingleCellItem>
	<!-- {#if $activeTab == 'Overview'}
		<FourCellItem></FourCellItem>
	{/if} -->
	<TwoCellItem datatype="orientation"></TwoCellItem>
	<TwoCellItem datatype="accel"></TwoCellItem>
	<SingleCellItem datatype="gasConcn"></SingleCellItem>
	<SingleCellItem datatype="velocity"></SingleCellItem>
</ul>
