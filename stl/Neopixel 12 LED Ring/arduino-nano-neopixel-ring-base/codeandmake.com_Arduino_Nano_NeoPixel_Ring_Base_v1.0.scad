/*
 * Copyright 2021 Code and Make (codeandmake.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Arduino Nano NeoPixel Ring Base by Code and Make (https://codeandmake.com/)
 *
 * https://codeandmake.com/post/arduino-nano-neopixel-ring-base
 *
 * Arduino Nano NeoPixel Ring Base v1.0 (5 May 2021)
 */

/* [General] */

// Base, Insert or Diffuser?
Part = "base"; // [base:Base, insert:Insert, diffuser:Diffuser, baseAndInsert:Base & Insert - Preview Only, all:All - Preview Only]

// The thickness of the material
Material_Thickness = 3; // [1:0.5:5]

/* [Base] */

// The diameter of the top opening
Opening_Diameter = 63.5; // [50:0.5:150]

// The depth of the top opening
Opening_Depth = 3; // [0:1:20]

// How much of a gap to add to the outside of the Arduino Nano
Arduino_Nano_Padding = 0.5; // [0:0.1:5]

// Make the base mostly hollow?
Hollow = 0; // [0:No, 1:Yes]

/* [Insert] */

// How much to remove from the outside of the insert
Insert_Padding = 0.5; // [0:0.1:5]

/* [NeoPixel] */

// The type of NeoPixel
NeoPixel = 12; // [12:NeoPixel 12-LED Ring, 16:NeoPixel 16-LED Ring, 24:NeoPixel 24-LED Ring]

// The diameter of the wire holes for the NeoPixel
NeoPixel_Wire_Hole_Diameter = 3; // [1:0.5:5]

// How much of a gap to add to the outside of the NeoPixel recess
NeoPixel_Outer_Padding = 0.5; // [0:0.1:5]

/* [Diffuser] */

// How much to remove from the outside of the diffuser
Diffuser_Padding = 0.5; // [0:0.1:5]

// The thickness of the diffuser
Diffuser_Thickness = 1; // [0.5:0.1:5]

module neoPixelRingBase() {
  $fn = 200;

  neoPixel12LEDRingOuterDiameter = 37;
  neoPixel16LEDRingOuterDiameter = 44.5;
  neoPixel24LEDRingOuterDiameter = 66;

  neoPixel12LEDRingOuterDiameterPadded = neoPixel12LEDRingOuterDiameter + (NeoPixel_Outer_Padding * 2);
  neoPixel16LEDRingOuterDiameterPadded = neoPixel16LEDRingOuterDiameter + (NeoPixel_Outer_Padding * 2);
  neoPixel24LEDRingOuterDiameterPadded = neoPixel24LEDRingOuterDiameter + (NeoPixel_Outer_Padding * 2);

  neoPixel12CablePositions = [0, 2, 6, 8];
  neoPixel16CablePositions = [0, 2, 7, 8, 12, 13];
  neoPixel24CablePositions = [0, 2, 10, 12, 14, 16];

  neoPixelHeight = 3.5;

  arduinoNanoLength = 45 + (Arduino_Nano_Padding * 2);
  arduinoNanoWidth = 18 + (Arduino_Nano_Padding * 2);
  arduinoNanoHeight = 8 + (Arduino_Nano_Padding * 2);
  arduinoNanoHypot = sqrt(pow(arduinoNanoLength, 2) + pow(arduinoNanoWidth, 2));

  powerSocketDiameter = 12;
  powerSocketOuterDiameter = 13.8;
  powerSocketNutDiameter = 18;
  powerSocketDepthRequired = max(powerSocketOuterDiameter, powerSocketNutDiameter);
  powerSocketCutoutLength = sqrt(pow(Opening_Diameter / 2, 2) - pow(powerSocketDepthRequired / 2, 2));

  baseDepth = max(arduinoNanoHeight, powerSocketDepthRequired);

  hollowRadius = (Opening_Diameter / 2) - Insert_Padding - Material_Thickness;

  module base() {
    difference() {
      union() {
        // base
        translate([0, 0, -(baseDepth + Material_Thickness)]) {
          cylinder(d = Opening_Diameter + (Material_Thickness * 2), h = baseDepth + Material_Thickness);
        }

        // lip
        rotate_extrude(convexity=10) {
          union() {
            translate([(Opening_Diameter) / 2, 0, 0]) {
              square([Material_Thickness, Material_Thickness + neoPixelHeight + Opening_Depth]);
            }
            translate([(Opening_Diameter + Material_Thickness) / 2, Material_Thickness + neoPixelHeight + Opening_Depth, 0]) {
              intersection() {
                translate([-Material_Thickness / 2, 0, 0]) {
                  square([Material_Thickness, Material_Thickness / 2]);
                }
                circle(d = Material_Thickness);
              }
            }
          }
        }

        // power socket outer ring
        translate([0, 0, -powerSocketDepthRequired / 2]) {
          rotate([-90, 0, 0]) {
            cylinder(d = powerSocketOuterDiameter, h = (Opening_Diameter / 2) + Material_Thickness);
          }
        }
      }

      // Arduino Nano internal cutout
      translate([-arduinoNanoLength / 2, -arduinoNanoWidth / 2, -baseDepth]) {
        cube(size=[arduinoNanoLength, arduinoNanoWidth, baseDepth + 1]);
      }

      // power socket internal cutout
      translate([-powerSocketDepthRequired / 2, 0, -baseDepth]) {
        cube(size=[powerSocketDepthRequired, powerSocketCutoutLength, baseDepth + 0.01]);
      }

      // power socket hole
      translate([0, 0, -powerSocketDepthRequired / 2]) {
        rotate([-90, 0, 0]) {
          cylinder(d = powerSocketDiameter, h = (Opening_Diameter / 2) + Material_Thickness + 1);
        }
      }

      // hollow
      if(Hollow) {
        intersection() {
          translate([0, 0, -baseDepth]) {
            cylinder(r = hollowRadius, h = baseDepth + 1);
          }

          union() {
            translate([(powerSocketDepthRequired / 2) + Material_Thickness, (arduinoNanoWidth / 2) + Material_Thickness, -baseDepth]) {
              cube([hollowRadius, hollowRadius, baseDepth + 1]);
            }

            mirror([1, 0, 0]) {
              translate([(powerSocketDepthRequired / 2) + Material_Thickness, (arduinoNanoWidth / 2) + Material_Thickness, -baseDepth]) {
                cube([hollowRadius, hollowRadius, baseDepth + 1]);
              }
            }
            
            translate([(arduinoNanoLength / 2) + Material_Thickness, -(arduinoNanoWidth / 2) - Material_Thickness - 0.5, -baseDepth]) {
              cube([hollowRadius, arduinoNanoWidth + (Material_Thickness * 2) + 1, baseDepth + 1]);
            }

            mirror([1, 0, 0]) {
              translate([(arduinoNanoLength / 2) + Material_Thickness, -(arduinoNanoWidth / 2) - Material_Thickness - 0.5, -baseDepth]) {
                cube([hollowRadius, arduinoNanoWidth + (Material_Thickness * 2) + 1, baseDepth + 1]);
              }
            }

            translate([-hollowRadius, -hollowRadius - (arduinoNanoWidth / 2) - Material_Thickness, -baseDepth]) {
              cube([hollowRadius * 2, hollowRadius, baseDepth + 1]);
            }
          }
        }
      }
    }
  }

  module wireHoles(neoPixelLEDRingOuterDiameter, positions) {
    for (i=[0:len(positions) - 1]) {
      rotate([0, 0, (360 / NeoPixel) * -positions[i]]) {
        hull() {
          translate([(neoPixelLEDRingOuterDiameter - NeoPixel_Wire_Hole_Diameter) / 2, 0, 0]) {
            cylinder(d = NeoPixel_Wire_Hole_Diameter, h = Material_Thickness + 1);
          }
          cylinder(d = NeoPixel_Wire_Hole_Diameter, h = Material_Thickness + 1);
        }
      }
    }
  }

  module insert() {
    difference() {
      cylinder(d = Opening_Diameter - (Insert_Padding * 2), h = Material_Thickness + neoPixelHeight);

      // outer
      translate([0, 0, Material_Thickness]) {
        if(NeoPixel == 12) {
          cylinder(d = neoPixel12LEDRingOuterDiameterPadded, h = neoPixelHeight + 1);
        }
        if(NeoPixel == 16) {
          cylinder(d = neoPixel16LEDRingOuterDiameterPadded, h = neoPixelHeight + 1);
        }
        if(NeoPixel == 24) {
          cylinder(d = neoPixel24LEDRingOuterDiameterPadded, h = neoPixelHeight + 1);
        }
      }

      // inner
      translate([0, 0, -0.5]) {
        if(NeoPixel == 12) {
          cylinder(d = arduinoNanoWidth, h = Material_Thickness + 1);
          wireHoles(neoPixel12LEDRingOuterDiameter, neoPixel12CablePositions);
        }
        if(NeoPixel == 16) {
          cylinder(d = arduinoNanoWidth, h = Material_Thickness + 1);
          wireHoles(neoPixel16LEDRingOuterDiameter, neoPixel16CablePositions);
        }
        if(NeoPixel == 24) {
          cylinder(d = arduinoNanoWidth, h = Material_Thickness + 1);
          wireHoles(neoPixel24LEDRingOuterDiameter, neoPixel24CablePositions);
        }
      }
    }
  }

  module diffuser() {
    translate([0, 0, Material_Thickness + neoPixelHeight]) {
      cylinder(d = Opening_Diameter - (Diffuser_Padding * 2), h = Diffuser_Thickness);
    }
  }

  module error(text) {
    echo(text);
    if($preview) {
      color([255/255, 0/255, 0/255]) {
        text(text, valign = "center", halign = "center");
      }
    }
  }

  if((NeoPixel == 12 && neoPixel12LEDRingOuterDiameterPadded > Opening_Diameter - (Insert_Padding * 2))
    || (NeoPixel == 16 && neoPixel16LEDRingOuterDiameterPadded > Opening_Diameter - (Insert_Padding * 2))
    || (NeoPixel == 24 && neoPixel24LEDRingOuterDiameterPadded > Opening_Diameter - (Insert_Padding * 2))) {
    error("ERROR: NeoPixel Ring diameter is larger than the opening");
  } else if(arduinoNanoHypot > Opening_Diameter) {
    error("ERROR: Arduino Nano is larger than the opening");
  } else {
    if(Part == "all") {
      if($preview) {
        color([255/255, 255/255, 255/255, 128/255]) {
          diffuser();
        }
        
        color([128/255, 128/255, 128/255]) {
          insert();
        }

        color([128/255, 128/255, 128/255]) {
          base();
        }
      } else {
        error("ERROR: You can only view 'All' parts in preview mode");
      }
    }

    if(Part == "baseAndInsert") {
      if($preview) {
        color([128/255, 128/255, 128/255]) {
          insert();
        }

        color([128/255, 128/255, 128/255]) {
          base();
        }
      } else {
        error("ERROR: You can only view 'Base and Insert' parts in preview mode");
      }
    }

    if(Part == "diffuser") {
      diffuser();
    }

    if(Part == "insert") {
      insert();
    }

    if(Part == "base") {
      base();
    }
  }
}

neoPixelRingBase();

echo("\r\tThis design is completely free and shared under a permissive license.\r\tYour support is hugely appreciated: codeandmake.com/support\r");