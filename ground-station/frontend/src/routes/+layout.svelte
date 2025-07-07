<script lang="ts">
	import '@src/app.css';
	import { rocketData } from '@src/store';
	import { Toaster } from '$lib/components/ui/sonner/index.js';
	import { connect_socket, get_startupdata, save_startupdata } from '@src/socket-interface';

	let { children } = $props();

	const connect_server = () => {
		connect_socket();
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

		if (window) {
			let latest_data = JSON.parse(window.sessionStorage.getItem('latest-data') || '{}');
			if (Object.keys(latest_data).length > 0) {
				rocketData.set(latest_data);
			}
		}
	});
</script>

<svelte:head>
	<title>Team Dyaus</title>
	<link rel="icon" href="/logo_dyaus.png" />
</svelte:head>

<Toaster></Toaster>

<div class="dark flex h-screen w-screen bg-background">
	{@render children()}
</div>
