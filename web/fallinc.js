(async () => {
  const CELL_WIDTH = 10;
  const CELL_HEIGHT = 16;
  const CONSOLE_WIDTH = 80;
  const CONSOLE_HEIGHT = 25;
  const ANSI_TO_RGB = {
      30 : [ 0, 0, 0 ],
      31 : [ 170, 0, 0 ],
      32 : [ 0, 170, 0 ],
      33 : [ 170, 85, 0 ],
      34 : [ 0, 0, 170 ],
      35 : [ 170, 0, 170 ],
      36 : [ 0, 170, 170 ],
      37 : [ 170, 170, 170 ],
      90 : [ 85, 85, 85 ],
      91 : [ 255, 85, 85 ],
      92 : [ 85, 255, 85 ],
      93 : [ 255, 255, 85 ],
      94 : [ 85, 85, 255 ],
      95 : [ 255, 85, 255 ],
      96 : [ 85, 255, 255 ],
      97 : [ 255, 255, 255 ],
  };
  const memory = new WebAssembly.Memory({ initial: 2 });
  const wasmFetchResponse = await fetch("./fallinc.wasm");
  const bytes = await wasmFetchResponse.arrayBuffer();
  const wasmModule = await WebAssembly.instantiate(bytes, {
    env: {
      memory,
      js_draw_console: (buf, cols, rows) => {
        const ctx = window["fallinc-canvas"].getContext("2d");

    ctx.font = `${CELL_HEIGHT}px monospace`;
    ctx.textBaseline = "top";
    const HEAPU8 = new Uint8Array(memory.buffer);
    const HEAP8 = new Int8Array(memory.buffer);
    for (let y = 0; y < rows; y++) {
      for (let x = 0; x < cols; x++) {
        const index = y * cols + x;
        const ptr = buf + index * 3;
        const ch = String.fromCharCode(HEAPU8[ptr]);
        const bgCode = HEAP8[ptr + 1];
        const fgCode = HEAP8[ptr + 2]; 
        const bg = ANSI_TO_RGB[bgCode];
        const fg = ANSI_TO_RGB[fgCode];
        ctx.fillStyle = `rgb(${bg[0]},${bg[1]},${bg[2]})`;
        ctx.fillRect(x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT);
        ctx.fillStyle = `rgb(${fg[0]},${fg[1]},${fg[2]})`;
        ctx.fillText(ch, x * CELL_WIDTH, y * CELL_HEIGHT);
      }
    }
        
      },
      js_log: (ptr) => {
        const buffer = new Uint8Array(memory.buffer);
        let end = ptr;
        while (buffer[end] !== 0) ++end;
        const bytes = buffer.subarray(ptr, end);
        const str = new TextDecoder("utf-8").decode(bytes);
        console.log(str);
      },
      js_initialize_canvas: () => {
        const canvas = window["fallinc-canvas"];
        canvas.width = CONSOLE_WIDTH * CELL_WIDTH;
        canvas.height = CONSOLE_HEIGHT * CELL_HEIGHT;
        canvas.style.imageRendering = "pixelated";
        const ctx = canvas.getContext("2d");
        ctx.imageSmoothingEnabled = false;
      }
    }
  });
  const { wasm_main } = wasmModule.instance.exports;
  wasm_main();
})();
