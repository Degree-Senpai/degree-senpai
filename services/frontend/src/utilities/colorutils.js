export function modifyHSLA(hsla, h, s, l, a) {
    let values = hsla.split('(')[1];
    values = values.split(')')[0];
    values = values.split(',');
    h = h + Number(values[0]);
    s = s + Number(values[1].split('%')[0]);
    l = l + Number(values[2].split('%')[0]);
    a = a + Number(values[3]);
    return `hsla(${h % 360}, ${s}%, ${l}%, ${a})`;
}