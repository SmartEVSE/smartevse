module slot(h, r, l, center = true)
    linear_extrude(height = h, convexity = 6, center = center)
        hull() {
            translate([l/2,0,0])
                circle(r = r, center = true);
            translate([-l/2,0,0])
                circle(r = r, center = true);
        }

module nut_trap(screw_r, nut_r, depth, horizontal = false, supported = false) {
    union() {
        if(horizontal) {
            if(screw_r)
                teardrop_plus(r = screw_r, h = 200, center = true);
            cylinder(r = nut_r + layer_height / 4, h = depth * 2, center = true, $fn = 6);
        }
        else {
            difference() {
                union() {
                    if(screw_r)
                        poly_cylinder(r = screw_r, h = 200, center = true);
                    cylinder(r = nut_r, h = depth * 2, center = true, $fn = 6);
                }
                if(supported)
                    translate([0, 0, depth - eta])
                        cylinder(r = nut_r, h = layer_height, center = false);
            }
        }
    }
} 

module head_trap(screw_r, nut_r, depth, supported = false) {
    union() {
          difference() {
                union() {
                    if(screw_r)
                        poly_cylinder(r = screw_r, h = 200, center = true);
                    cylinder(r = nut_r, h = depth * 2, center = true);
                }
                if(supported)
                    translate([0, 0, depth - eta])
                        cylinder(r = nut_r, h = layer_height, center = false);
            }
        }
} 

module rounded_square(w, h, r)
{
    union() {
        square([w - 2 * r, h], center = true);
        square([w, h - 2 * r], center = true);
        for(x = [-w/2 + r, w/2 - r])
            for(y = [-h/2 + r, h/2 - r])
                translate([x, y])
                    circle(r = r);
    }
}

module rounded_rectangle(size, r, center = true)
{
    w = size[0];
    h = size[1];
    linear_extrude(height = size[2], center = center)
        rounded_square(size[0], size[1], r);
}
