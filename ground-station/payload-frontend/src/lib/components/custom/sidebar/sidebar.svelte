<script lang="ts">
	import * as Card from '@src/lib/components/ui/card';
	import LastCommandDisplay from './last-command-display.svelte';
	import CommandSender from './command-sender.svelte';
	import Logos from './logos.svelte';
	import MissionProgress from './mission-progress.svelte';
	import { connect_socket, disconnect_socket } from '@src/socket-interface';
	import Button from '$lib/components/ui/button/button.svelte';

	let timeIST = $state();
	let dateToday = $state(new Date().toLocaleDateString());
	setInterval(() => {
		timeIST = new Date().toLocaleTimeString();
	}, 1000);
</script>

<div class="sidebar flex w-full flex-col gap-4">
	<Logos></Logos>

	<LastCommandDisplay></LastCommandDisplay>

	<CommandSender></CommandSender>

	<MissionProgress></MissionProgress>

	<Card.Root class="time-date flex content-center items-center gap-8 p-4 ">
		<ul>
			<li>
				<span>TIME (IST): </span>
				<span>{timeIST}</span>
			</li>
			<li>
				<span>UPTIME: </span>
				<span>00:00:00</span>
			</li>
			<li>
				<span>DATE:</span>
				<span>{dateToday}</span>
			</li>
		</ul>
	</Card.Root>

	<Card.Root class="team-info flex content-center items-center gap-8 p-4 ">
		<ul>
			<li>
				<span>Team ID: </span>
				<span>2024ASI-ROCKETRY-026</span>
			</li>
			<li>
				<span>Team Name:</span>
				<span>TEAM DYAUS</span>
			</li>
		</ul>
	</Card.Root>
</div>

<style>
</style>
