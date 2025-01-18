<script lang="ts">
	import '@src/app.css';
	import Button from '@src/lib/components/ui/button/button.svelte';
	import { activeTab } from '@src/store';
	import Sidebar from '@src/lib/components/custom/sidebar/sidebar.svelte';
	import { Toaster } from '$lib/components/ui/sonner/index.js';

	let { children } = $props();
</script>

<Toaster></Toaster>

<div class="bg-background dark flex h-screen w-screen gap-4 overflow-hidden p-8">
	<div class="flex flex-col gap-3">
		<nav>
			<ul class="flex gap-4">
				<li>
					<Button
						variant="link"
						class={$activeTab == 'Overview' ? 'underline' : ''}
						href="/overview">Overview</Button
					>
				</li>
				<li>
					<Button variant="link" class={$activeTab == 'Graphs' ? 'underline' : ''} href="/graphs"
						>Graphs</Button
					>
				</li>
				<li>
					<Button variant="link" class={$activeTab == 'CSV' ? 'underline' : ''} href="/csv"
						>CSV</Button
					>
				</li>
			</ul>
		</nav>

		<div class="flex h-full w-full flex-col gap-4">
			{@render children()}
		</div>
	</div>

	{#if $activeTab != 'CSV'}
		<Sidebar></Sidebar>
	{/if}
</div>
