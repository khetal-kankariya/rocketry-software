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
				'altitude',
				'pressure',
				'temperature',
				'voltage',
				'gasConcn',
				'gnss',
				'velocity',
				'accel',
				'orientation'
			];
		}
	});
</script>

<ul class="datagrid grid w-full min-w-[1392px] grid-cols-4 gap-4 p-0">
	{#each gridDataNames as datatype}
		{#if datatype == 'gnss' && $activeTab == 'Overview'}
			<FourCellItem {datatype}></FourCellItem>
		{:else if datatype == 'orientation' || datatype == 'accel'}
			<TwoCellItem {datatype}></TwoCellItem>
		{:else if $allDataEverything[datatype].multiParameter == false}
			<SingleCellItem {datatype}></SingleCellItem>
		{/if}
	{/each}
</ul>
