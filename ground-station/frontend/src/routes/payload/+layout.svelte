<script lang="ts">
	import '@src/app.css';
	import Button from '@src/lib/components/ui/button/button.svelte';
	import { activeTab, GCStype, payloadData, server_connected, server_connecting } from '@src/store';
	import Sidebar from '@src/lib/components/custom/payload/sidebar/sidebar.svelte';
	import { Toaster } from '$lib/components/ui/sonner/index.js';
	import {
		connect_socket,
		disconnect_socket,
		get_csv_data,
		get_startupdata,
		save_startupdata
	} from '@src/socket-interface';
	import { LoaderCircle } from 'lucide-svelte';

	let { children } = $props();

	const connect_server = () => {
		connect_socket();
	};
	const disconnect_server = () => {
		disconnect_socket();
	};

	$effect(() => {
		const last_server_status = window.sessionStorage.getItem('server-connected');
		if (last_server_status == 'true') connect_server();
	});

	$effect.root(() => {
		import('@src/dataupdater');
		get_startupdata();
		save_startupdata();
		import('@src/socket-interface');
		$GCStype = 'payload';

		if (window) {
			let latest_data = JSON.parse(window.sessionStorage.getItem('latest-data') || '{}');
			if (Object.keys(latest_data).length > 0) {
				payloadData.set(latest_data);
			}
		}
	});
</script>

<svelte:head>
	<title>Team Dyaus</title>
	<link rel="icon" href="/logo_dyaus.png" />
</svelte:head>

<Toaster></Toaster>

<div class="flex h-screen w-screen gap-4 p-8">
	<div class="flex h-full w-full flex-col gap-3">
		<nav>
			<ul class="flex gap-4">
				<li>
					<Button
						variant="link"
						class={$activeTab == 'Overview' ? 'underline' : ''}
						href="/payload/overview"
					>
						Overview
					</Button>
				</li>
				<li>
					<Button
						variant="link"
						class={$activeTab == 'Graphs' ? 'underline' : ''}
						href="/payload/graphs"
					>
						Graphs
					</Button>
				</li>
				<li>
					<Button variant="link" class={$activeTab == 'CSV' ? 'underline' : ''} href="/payload/csv">
						CSV
					</Button>
				</li>

				<li class="ml-auto">
					<span>Server: </span>
					{#if $server_connecting}
						<Button class="w-40" disabled>
							<LoaderCircle class="animate-spin">In Process</LoaderCircle>
						</Button>
					{:else if $server_connected}
						<Button
							onclick={disconnect_server}
							variant="default"
							class="w-40 bg-green-400 text-sm hover:bg-green-200"
						>
							Connected
						</Button>
					{:else}
						<Button
							onclick={connect_server}
							variant="destructive"
							class="w-40 text-sm hover:bg-red-800"
						>
							Not Connected
						</Button>
					{/if}
				</li>

				{#if $activeTab == 'CSV'}
					<li class="">
						<Button class="ml-auto w-min" onclick={get_csv_data}>Refresh</Button>
					</li>
				{/if}
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
