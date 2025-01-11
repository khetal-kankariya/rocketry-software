export const ssr = false;

export const load = ({ params }) => {
	return { field_name: params.field_name };
};
