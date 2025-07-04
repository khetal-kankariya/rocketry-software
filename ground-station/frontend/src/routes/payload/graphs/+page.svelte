<script lang="ts">
	import DataGrid from '@src/lib/components/custom/payload/datagrid/data-grid.svelte';
	import { activeTab, GCS_started } from '@src/store';

	$effect(() => {
		$activeTab = 'Graphs';
		$GCS_started = true;
	});
</script>

{#if $activeTab == 'Graphs'}
	<DataGrid></DataGrid>
{/if}
