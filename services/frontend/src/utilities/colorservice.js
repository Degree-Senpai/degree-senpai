class Color {
    constructor(hue, saturation, lightness, alpha) {
        this.hue = hue;
        this.saturation = saturation;
        this.lightness = lightness;
        this.alpha = alpha;
    }

    interpolate(otherColor, factor, longerLoop=false) {
        clockwiseDistance = (otherColor.hue - this.hue + 360) %  360;
        counterclockwiseDistance = (this.hue - otherColor.hue + 360) % 360;
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

class ColorPalette {
    constructor(main, highlight, layerouter, layermid, layerinner, iconmain, iconsub, iconsmall) {
        this.main = main;
        this.highlight = highlight;
        this.layerouter = layerouter;
        this.layermid = layermid;
        this.layerinner = layerinner;
        this.iconmain = iconmain;
        this.iconsub = iconsub;
        this.iconsmall = iconsmall;
    }
}

export class Colors {
    /*
        This is the main class for fetching color data. It contains a single object, ColorPalette, which
        contains all the color definitions.

        it also contains functions to compute colors.
    */
    constructor(colorPalette = null) {
        this.palette = colorPalette;
        if (this.palette == null) {
            this.palette = new ColorPalette();
        }
    }

    gradientSampled(factor, color1 = null, color2 = null, longerLoop = false) {
        /* factor dictates the location within the gradient to sample and return.

        color1 and color2 is the two colors to sample between. If they're null,
        the full rainbow spectrum (red to violet) is used.

        longerLoop means we interpolate using the longer way between the hues instead.
        For example, to get the full rainbow we need it to be true since otherwise
        we just end up with an entirely red gradient, since start and end are next
        to each other.
         */
        if (color1 == null) {
            color1 = new Color(0, 10, 50, 1);
        }
        if (color2 == null) {
            color2 = new Color(359, 10, 50, 1);
        }

        newColor = color1.interpolate(color2, factor, longerLoop);
        return newColor
    }

    gradientSpectrum(factor, color1 = null, color2 = null, smallerLoop = true, numIndex) {
        /* samples numIndex amount evenly from gradient */
        let colors = [];
        for (let i = 0; i < numIndex; ++i) {
            colors.push(this.gradientSampled(factor, color1, color2, smallerLoop));
        }
        return colors
    }
}