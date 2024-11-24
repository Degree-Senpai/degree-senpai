import {Color as ColorLib} from 'color'

/*
SCROLL DOWN TO class Colors for the main colorservice class. Colors is responsible for storing the color palette as
attribute "palette" and computed functions, including gradientSampled and gradientFull.

To initialize this class for use, create a ColorPalette class and fill in necessary colors with 
class Color(hue, saturation, lightness, alpha).
*/

export class Color {
    constructor(hue=0, saturation=0, lightness=0, alpha=1, hex=-1) {
        this.hue = hue;
        this.saturation = saturation;
        this.lightness = lightness;
        this.alpha = alpha;
        if (hex != -1) {
            const hsl = ColorLib(hex).hsl().object();
            this.hue = hsl.h;
            this.saturation = hsl.s;
            this.lightness = hsl.l;
        }
    }

    interpolate(otherColor, factor, longerLoop=false) {
        let clockwiseDistance = (otherColor.hue - this.hue + 360) %  360;
        let counterclockwiseDistance = (this.hue - otherColor.hue + 360) % 360;
        console.log(`clockwise distance between ${this.hue} and ${otherColor.hue}: ${clockwiseDistance}`);
        console.log(`counterclockwise distance between ${this.hue} and ${otherColor.hue}: ${counterclockwiseDistance}`);

        let newHue = 0;
        if (clockwiseDistance > counterclockwiseDistance == longerLoop) {
            // use clockwise interpolation, meaning we increase hue from this color
            newHue = (this.hue + clockwiseDistance * factor) % 360;
        }
        else {
            newHue = (this.hue - counterclockwiseDistance * factor + 360) % 360;
        }
        return new Color(
            newHue,
            this.saturation * (1 - factor) + otherColor.saturation * (factor),
            this.lightness * (1 - factor) + otherColor.lightness * (factor),
            this.alpha * (1 - factor) + otherColor.alpha * (factor)
        )
    }

    exportString() {
        return `hsla(${this.hue}, ${this.saturation}%, ${this.lightness}%, ${this.alpha}`;
    }
}

export class ColorPalette {
    constructor(main, highlight, block1, block2, icon1, icon2) {
        this.main = main;
        this.highlight = highlight;
        this.block1 = block1;
        this.block2 = block2;
        this.icon1 = icon1;
        this.icon2 = icon2;
    }
}

export class Colors {
    /*
        This is the main class for fetching color data. It contains a single object, ColorPalette, which
        contains all the color definitions.

        it also contains functions to compute colors.
    */
    constructor(colorPalette = null, customColors = null) {
        this.palette = colorPalette;
        this.customColors = customColors;
        if (this.palette == null) {
            this.palette = new ColorPalette();
        }
        if (this.customColors == null) {
            this.customColors = {};
        }
        this.randomColors = {}; // keep track of randomly assigned colors
    }

    addCustomColor(id, color) {
        this.customColors[id] = color;
    }

    removeCustomColor(id) {
        if (this.customColors[id] != null) {
            delete this.customColors.id;
        }
    }

    getCustomColor(id) {
        return this.customColors[id];
    }

    gradientSampled(factor, color1 = null, color2 = null, longerLoop = false, random = false) {
        /* factor dictates the location within the gradient to sample and return.

        color1 and color2 is the two colors to sample between. If they're null,
        the full rainbow spectrum (red to violet) is used.

        longerLoop means we interpolate using the longer way between the hues instead.
        For example, to get the full rainbow we need it to be true since otherwise
        we just end up with an entirely red gradient, since start and end are next
        to each other.

        if random is used, then factor becomes an ID where giving the same factor always
        returns the same random color as long as this class is maintained.
         */
        let bothNull = 0;
        if (color1 == null) {
            color1 = new Color(0, 10, 50, 1);
            bothNull++;
        }
        if (color2 == null) {
            color2 = new Color(359, 10, 50, 1);
            bothNull++;
        }
        if (bothNull == 2) {
            longerLoop = true;
        }

        if (random) {
            if (this.randomColors[factor] != null) {
                factor = this.randomColors[factor];
            }
            else {
                let randomFactor = Math.random();
                this.randomColors[factor] = randomFactor;
                factor = randomFactor;
            }
        }

        let newColor = color1.interpolate(color2, factor, longerLoop);
        return newColor
    }

    gradient(num, color1 = null, color2 = null, longerLoop = false) {
        /* samples numIndex amount evenly from gradient */
        let colors = [];
        for (let i = 0; i < num; ++i) {
            colors.push(this.gradientSampled(i, color1, color2, longerLoop));
        }
        return colors
    }
}

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
