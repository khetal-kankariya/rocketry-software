<script lang="ts">
	import { activeTab, GCStype } from '@src/store';
	import SingleCellItem from '@src/lib/components/custom/payload/cell-item/one-cell.svelte';
	import TwoCellItem from '@src/lib/components/custom/payload/cell-item/two-cell.svelte';
</script>

<ul class="datagrid grid w-full min-w-[1392px] grid-cols-4 gap-4 p-0">
	{#if $activeTab == 'Overview'}
		<SingleCellItem datatype="team_id"></SingleCellItem>
		<SingleCellItem datatype="packet_count"></SingleCellItem>
		<SingleCellItem datatype="temperature"></SingleCellItem>
		<SingleCellItem datatype="gas_concentration"></SingleCellItem>
		<SingleCellItem datatype="altitude"></SingleCellItem>
		<SingleCellItem datatype="pressure"></SingleCellItem>
		<SingleCellItem datatype="velocity"></SingleCellItem>
		<TwoCellItem datatype="orientation"></TwoCellItem>
		<TwoCellItem datatype="accel"></TwoCellItem>
	{:else}
		<SingleCellItem datatype="altitude"></SingleCellItem>
		<SingleCellItem datatype="pressure"></SingleCellItem>
		<SingleCellItem datatype="velocity"></SingleCellItem>
		<TwoCellItem datatype="orientation"></TwoCellItem>
		<TwoCellItem datatype="accel"></TwoCellItem>
		<SingleCellItem datatype="temperature"></SingleCellItem>
		<SingleCellItem datatype="gas_concentration"></SingleCellItem>
	{/if}
</ul>
