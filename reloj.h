// reloj.h

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120; // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;                    // for next 1 second timeout
uint8_t hh = 0, mm = 5, ss = 10; 
const float coef = 0.0174532925;

boolean initial = 1;

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9') {
        v = *p - '0';
    }
    return 10 * v + *++p - '0';
}

void setup_reloj(void) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
    tft.fillCircle(120, 120, 118, TFT_GREEN);
    tft.fillCircle(120, 120, 110, TFT_BLACK);
    for (int i = 0; i < 360; i += 30)     // Draw 12 lines
      {
      sx = cos((i - 90) * coef);
      sy = sin((i - 90) * coef);
      x0 = sx * 114 + 120;
      yy0 = sy * 114 + 120;
      x1 = sx * 100 + 120;
      yy1 = sy * 100 + 120;
      tft.drawLine(x0, yy0, x1, yy1, TFT_GREEN);
      }
    // Draw 60 dots
    for (int i = 0; i < 360; i += 6) 
      {
      sx = cos((i - 90) * coef);
      sy = sin((i - 90) * coef);
      x0 = sx * 102 + 120;
      yy0 = sy * 102 + 120;
      // Draw minute markers
      tft.drawPixel(x0, yy0, TFT_WHITE);
      // Draw main quadrant dots
      if (i == 0 || i == 90 || i == 180 || i == 270) 
        {
        tft.fillCircle(x0, yy0, 2, TFT_WHITE);
        }
      }
    tft.fillCircle(120, 121, 3, TFT_WHITE);
    tft.setCursor(210, 110, 2); tft.print(3); 
    tft.setCursor(115, 205, 2); tft.print(6); 
    tft.setCursor(25, 110, 2); tft.print(9); 
    tft.setCursor(115, 20, 2); tft.print(12); 
    targetTime = millis() + 1000;
}

void loop_reloj() {
    if (targetTime < millis()) {
        targetTime += 1000;
        ss++;              // Advance second
        if (ss == 60) {
            ss = 0;
            mm++;            // Advance minute
            if (mm > 59) {
                mm = 0;
                hh++;          // Advance hour
                if (hh > 23) {
                    hh = 0;
                }
            }
        }

        // Pre-compute hand degrees, x & y coords for a fast screen update
        sdeg = ss * 6;                // 0-59 -> 0-354
        mdeg = mm * 6 + sdeg * 0.01666667; // 0-59 -> 0-360 - includes seconds
        hdeg = hh * 30 + mdeg * 0.0833333; // 0-11 -> 0-360 - includes minutes and seconds
        hx = cos((hdeg - 90) * coef);
        hy = sin((hdeg - 90) * coef);
        mx = cos((mdeg - 90) * coef);
        my = sin((mdeg - 90) * coef);
        sx = cos((sdeg - 90) * coef);
        sy = sin((sdeg - 90) * coef);

        if (ss == 0 || initial) {
            initial = 0;
            // Erase hour and minute hand positions every minute
            tft.drawLine(ohx, ohy, 120, 121, TFT_BLACK);
            ohx = hx * 62 + 121;
            ohy = hy * 62 + 121;
            tft.drawLine(omx, omy, 120, 121, TFT_BLACK);
            omx = mx * 84 + 120;
            omy = my * 84 + 121;
        }

        // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
        tft.drawLine(osx, osy, 120, 121, TFT_BLACK);
        osx = sx * 90 + 121;
        osy = sy * 90 + 121;
        tft.drawLine(osx, osy, 120, 121, TFT_RED);
        tft.drawLine(ohx, ohy, 120, 121, TFT_WHITE);
        tft.drawLine(omx, omy, 120, 121, TFT_WHITE);
        tft.drawLine(osx, osy, 120, 121, TFT_RED);
        tft.fillCircle(120, 121, 3, TFT_RED);
    }
}

