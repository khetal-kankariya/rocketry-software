<script lang="ts">
	import SpeedometerPointer from './speedometer-pointer.svelte';
	import SpeedometerBar from './speedometer-bar.svelte';
	import { rocketData } from '@src/store';
	import { maxSpeed, minSpeed } from './store';

	let midSpeed = $derived($minSpeed + $maxSpeed / 2);

	$effect(() => {
		if ($rocketData?.velocity > $maxSpeed) $maxSpeed = $rocketData.velocity;
		if ($rocketData?.velocity < $minSpeed) $minSpeed = $rocketData.velocity;
	});
</script>

<div class="speedometer relative grid h-[1.5rem] w-full px-4">
	<ul class="major-values absolute mb-2 flex w-full justify-between px-1 text-xs text-white">
		<li class="-rotate-45">{$minSpeed}</li>
		<li class="-rotate-45">{midSpeed}</li>
		<li class="-rotate-45">{$maxSpeed}</li>
	</ul>
	<div class="absolute flex w-full justify-between"></div>
	<SpeedometerBar></SpeedometerBar>
	<div class="speed-pointer-bg absolute top-0 mx-4 h-full w-full">
		<SpeedometerPointer></SpeedometerPointer>
	</div>
</div>

<style lang="scss">
	.speedometer {
		// background-image: linear-gradient(
		// 	to right,
		// 	rgba(255, 0, 0, 0) 0%,
		// 	rgb(255, 0, 0) calc(16px),
		// 	yellow 30%,
		// 	green 50%,
		// 	yellow 80%,
		// 	rgb(255, 0, 0) calc(100% - 16px),
		// 	rgba(255, 0, 0, 0) 100%
		// );

		// background-image: url(/speedometer-bar.svg);

		.speed-pointer-bg {
			width: calc(100% - 32px);
		}
	}
</style>
