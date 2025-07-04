<script lang="ts">
	import { payloadData } from '@src/store';
	import { minSpeed, maxSpeed } from './store';

	let pointerEl: HTMLDivElement;

	$effect(() => {
		pointerEl.style.left = `calc(${(($payloadData?.velocity - $minSpeed) / ($maxSpeed - $minSpeed)) * 100}%)`;
	});
</script>

<div class="speed-pointer z-20 transition-all" bind:this={pointerEl}>
	<svg width="8" height="32" viewBox="0 0 16 64" fill="none" xmlns="http://www.w3.org/2000/svg">
		<path
			d="M8.78223 62.964L0.525443 49.4016L8.77652 3.13431L15.4829 49.4177L8.78223 62.964Z"
			fill="white"
			stroke="white"
		/>
	</svg>

	<span class="speed-num"> {$payloadData?.velocity} </span>
</div>

<style lang="scss">
	.speed-pointer {
		position: absolute;
		top: 0px;
		translate: -50% 0;
		height: 32px;
		width: 8px;
		transition-duration: 500ms;

		span {
			position: absolute;
			left: 50%;
			top: 60%;
			translate: -50% -50%;
			// height: 16px;
			font-size: 8px;
			color: white;
			text-align: center;
			width: 1.3em;
			background: hsl(222.2 84% 4.9%);
			// border: 1px solid #fff;
		}
	}
</style>
