//
// Sensorbox, connects up to 4 current transformers, and transmits data over rs485 to smart EVSE
// Top and bottom part 
//
// uses nopheads polyholes to make hex nut traps and holes
// 
// (c) 2013 Michael Stegen / Stegen Electronics

include <polyholes.scad>
include <teardrops.scad>
include <utils.scad>

$fn=50;

box_thickness = 1.8;
height = 8;
top_height = 8;
length=65.5;
width=46.5;
ctoffset=0;


layer_height = 0.3;					// layer height when printing, for supported holes
eta = 0.01;

M3_head_depth = 4;					// hex m3 bolt
M3_head_radius = 6.3 / 2;				// hex m3 bolt


M3_tap_radius = 2.5 / 2;
M3_clearance_radius = 3.3 / 2;
M3_nut_radius = 6.5 / 2;
M3_nut_trap_depth = 6;




module sensor_back() {
difference() {
			rounded_rectangle([length,width,height],3, center = false);
			difference() {
				translate([0,0,box_thickness])
				rounded_rectangle([length-(box_thickness*2),width-(box_thickness*2),height],2, center = false);

				translate([(length/2)-7,(width/2)-1,height-1])		// make clips on both sides
				rotate([0,90,0]) 
				cylinder(r=1,h=6, center=true);
				translate([-((length/2))+7,(width/2)-1,height-1])
				rotate([0,90,0]) 
				cylinder(r=1,h=6, center=true);
				translate([-((length/2))+7,-((width/2))+1,height-1])
				rotate([0,90,0]) 
				cylinder(r=1,h=6, center=true);
				translate([(length/2)-7,-((width/2))+1,height-1])
				rotate([0,90,0]) 
				cylinder(r=1,h=6, center=true);
			}

		
			translate([17+ctoffset,width/2-1,height])				// make holes for CTs
			rotate([90,0,0]) 
			cylinder(r=3.1,h=5, center=true);

			translate([5.7+ctoffset,width/2-1,height])
			rotate([90,0,0]) 
			cylinder(r=3.1,h=5, center=true);

			translate([-5.7+ctoffset,width/2-1,height])
			rotate([90,0,0]) 
			cylinder(r=3.1,h=5, center=true);

			translate([-17+ctoffset,width/2-1,height])
			rotate([90,0,0]) 
			cylinder(r=3.1,h=5, center=true);

			translate([(length/2)-1,0,top_height-0.2])			// make hole for 4 way header
			cube([4,15.2,7.5], center=true);


			}
}


module sensor_top() {
union(){

	difference() {
			union(){
				rounded_rectangle([length,width,top_height],3, center = false);			
				rounded_rectangle([length-(box_thickness*2)-0.15,width-(box_thickness*2)-0.15,height+2.2],2, center = false);
			}
			difference() {
				translate([0,0,box_thickness])
				rounded_rectangle([length-(box_thickness*4),width-(box_thickness*4),top_height+3],2, center = false);
				
			}

			translate([0,(width/2)-(box_thickness*2),box_thickness+2+(height/2)])
			cube([length-20,box_thickness*2,height+4],center=true);

			translate([0,-(width/2)+(box_thickness*2),box_thickness+2+(height/2)])
			cube([length-20,box_thickness*2,height+4],center=true);

			translate([(length/2)-(box_thickness*2),0,box_thickness+2+(height/2)])
			cube([box_thickness*2,width-20,height+4],center=true);

			translate([-(length/2)+(box_thickness*2),0,box_thickness+2+(height/2)])
			cube([box_thickness*2,width-20,height+4],center=true);


			translate([(length/2)-6,(width/2)-1,height+1])		// make clips on both sides
			rotate([0,90,0]) 
			cylinder(r=1,h=10, center=true);
			translate([-((length/2))+6,(width/2)-1,height+1])
			rotate([0,90,0]) 
			cylinder(r=1,h=10, center=true);
			translate([-((length/2))+6,-((width/2))+1,height+1])
			rotate([0,90,0]) 
			cylinder(r=1,h=10, center=true);
			translate([(length/2)-6,-((width/2))+1,height+1])
			rotate([0,90,0]) 
			cylinder(r=1,h=10, center=true);



		
			translate([17-ctoffset,width/2-1,top_height])				// make holes for CTs
			rotate([90,0,0]) 
			cylinder(r=3.1,h=6, center=true);

			translate([5.7-ctoffset,width/2-1,top_height])
			rotate([90,0,0]) 
			cylinder(r=3.1,h=6, center=true);

			translate([-5.7-ctoffset,width/2-1,top_height])
			rotate([90,0,0]) 
			cylinder(r=3.1,h=6, center=true);

			translate([-17-ctoffset,width/2-1,top_height])
			rotate([90,0,0]) 
			cylinder(r=3.1,h=6, center=true);

			translate([-(length/2)+1,0,height+0.2])				// make hole for 4 way header
			cube([6,15.2,7.5], center=true);


		} // end of difference

	//translate([-2.75,-4.75,height/2])						// light pipe for LED
	//cylinder(r=1.5,h=height,center=true);

 	} // end of union
}


translate([0,25,0])
 sensor_back();
translate([0,-25,0])
 sensor_top();

