<script>
	import { missionProgress } from '@src/store';
	import { Progress } from '$lib/components/ui/progress';
</script>

<!-- <Card.Root
	class="mission-progress h-12 w-full  rounded-2xl border border-[#444] bg-background"
	style="
background: #1A1A1A;
box-shadow: inset -8px 0px 8px rgba(0, 0, 0, 0.25), inset 8px 0px 8px rgba(0, 0, 0, 0.25), inset 0px -8px 8px rgba(0, 0, 0, 0.25);
"
>
	<div
		class="progress-bar h-full rounded-s-2xl border border-white bg-accent shadow-sm shadow-white"
		style={`width: ${$missionProgress}%;`}
	></div>
</Card.Root> -->

<Progress class="h-8 rounded-lg" value={$missionProgress}></Progress>
