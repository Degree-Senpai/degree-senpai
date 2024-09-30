export function newArray(dims) {
    // generate an array of a specific shape
    let array = Array(dims[0]);
    if (dims.length == 1) {
        return array;
    }
    let new_dims = dims.slice(1);
    for (let i = 0; i < array.length; ++i) {
        array[i] = newArray(new_dims);
    }
    return array;
}