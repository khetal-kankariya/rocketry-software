<script lang="ts">
	import '@src/app.css';
	import Button from '@src/lib/components/ui/button/button.svelte';
	import { activeTab, GCS_started, server_connected, server_connecting } from '@src/store';
	import Sidebar from '@src/lib/components/custom/sidebar/sidebar.svelte';
	import { Toaster } from '$lib/components/ui/sonner/index.js';
	import { connect_socket, disconnect_socket, get_csv_data } from '@src/socket-interface';
	import { onMount } from 'svelte';
	import { LoaderCircle } from 'lucide-svelte';

	let { children } = $props();

	const connect_server = () => {
		connect_socket();
	};
	const disconnect_server = () => {
		disconnect_socket();
	};

	// onMount(() => {
	// 	$server_connected = Boolean(window.sessionStorage.getItem('server-connected'));

	// 	if ($server_connected) connect_server();
	// });
</script>

<svelte:head>
	<title>ssss</title>
	<link rel="icon" href="/logo_dyaus.png" />
</svelte:head>

<Toaster></Toaster>

<div class="bg-background dark flex h-screen w-screen gap-4 p-8">
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

				<li class="ml-auto">
					<span>Server: </span>
					{#if $server_connecting}
						<Button disabled>
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

	{#if $activeTab != 'CSV' && GCS_started}
		<Sidebar></Sidebar>
	{/if}
</div>
