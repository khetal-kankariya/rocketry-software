<script lang="ts">
	import DataGrid from '@src/lib/components/custom/datagrid/data-grid.svelte';
	import { activeTab } from '@src/store';

	$effect(() => {
		$activeTab = 'Graphs';
	});
</script>

{#if $activeTab == 'Graphs'}
	<DataGrid></DataGrid>
{/if}
