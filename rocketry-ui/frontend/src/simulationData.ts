import type { LatLngExpression } from 'leaflet';
import { readable, writable, type Readable, type Writable } from 'svelte/store';

export const sim_coords: Writable<LatLngExpression[]> = writable([
	[23.129188892555288, 72.54200569857852],
	[23.140101, 72.53891],
	[23.133529, 72.567167],
	[23.112516415112626, 72.5547515602336]
]);
