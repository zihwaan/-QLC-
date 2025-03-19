module DataTransformer #(
    parameter DATA_WIDTH = 8,
    parameter LFSR_WIDTH = 24   
)(
    input wire clk,
    input wire reset_n,
    input wire req, 
    input wire  [LFSR_WIDTH-1:0] seed,    
    input wire  [DATA_WIDTH-1:0] data_in, 
    output wire [DATA_WIDTH-1:0] data_out,
    output wire ack  
);
    reg [LFSR_WIDTH-1:0] lfsr_state;
    reg [DATA_WIDTH-1:0] transformed_data;
    reg [2:0] bit_counter;
    reg ack_reg;

    always @(posedge clk or negedge reset_n) begin
        if (!reset_n) begin
            lfsr_state <= 24'b0;
            transformed_data <={DATA_WIDTH{1'b0}};  
            bit_counter <= 3'b0; 
            ack_reg <= 0;         
        end else begin
            if( req) begin
                lfsr_state <= seed;
                if(bit_counter < DATA_WIDTH ) begin
                    transformed_data[bit_counter ] <= data_in[bit_counter] ^ lfsr_state[0];
                    lfsr_state <= {lfsr_state[LFSR_WIDTH-2:0], (lfsr_state[24] ^ lfsr_state[22] ^ lfsr_state[21] ^ lfsr_state[19])};
                    bit_counter <= bit_counter + 1; 
                    ack_reg <= 1;
                      
                end 
            end else begin
                bit_counter <= 3'b0;
                lfsr_state <= 24'b0; 
                ack_reg <= 0;
            end
                
        end 
    end

    assign ack = ack_reg;
    assign data_out  = transformed_data;

endmodule