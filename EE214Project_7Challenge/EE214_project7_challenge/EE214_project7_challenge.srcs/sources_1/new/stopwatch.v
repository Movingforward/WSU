module stop_watch(    
    output [7:0] seg, 
    output [7:0] an,
    input start,
    input stop,
    input inc,
    input clk,
    input rst
    );        
    //clock divider    
    wire clk_div;    
    clk_divider devi(         
        .clk (clk),         
        .rst (rst),         
        .clk_div (clk_div)         
        );        
    //controller    
    wire Run;    
    Controller cont(        
        .Stop(stop),        
        .Start(start),        
        .Inc(inc),        
        .Run(Run),        
        .clk(clk_div),        
        .rst(rst)        
        );        
    //4 digit decimal counter    
    wire [3:0]Dig0;    
    wire [3:0]Dig1;    
    wire [3:0]Dig2;    
    wire [3:0]Dig3;        
        fourbit_counter four_bit(        
            .clk(clk_div),        
            .run(Run),        
            .rst(rst),        
            .Dig0(Dig0),        
            .Dig1(Dig1),        
            .Dig2(Dig2),        
            .Dig3(Dig3)        
            );        
        //4 to 1 mux, bussing 4    
        wire [3:0] num;    
        wire [1:0] twobitCout;        
        
        twobit_counter two_bit(        
            .clk(clk_div),        
            .rst(rst),        
            .Q(twobitCout)        
            );            
        
        
        mux_4_1 four_to_one(        
            .I0 (Dig0),        
            .I1 (Dig1),        
            .I2 (Dig2),        
            .I3 (Dig3),        
            .Sel (twobitCout),        
            .Y(num)        
            );        
        //2 bit counter              
        
        //2 to 4 AN decoder     
        assign an =     {
            (twobitCout == 2'b00) ? 8'b11111110:                 
            (twobitCout == 2'b01) ? 8'b11111101:                 
            (twobitCout == 2'b10) ? 8'b11111011:                 
            (twobitCout == 2'b11) ? 8'b11110111:                 
            8'b11111111 }
        ;        
        //7 seg decoder     
        assign seg = 
        { (num == 4'b0000) ? 8'b11000000:   // 0                                      
        //  .gfedcba                    
        (num == 4'b0001) ? 8'b11111001:   // 1                    
        (num == 4'b0010) ? 8'b10100100:   // 2                    
        (num == 4'b0011) ? 8'b10110000:   // 3                    
        (num == 4'b0100) ? 8'b10011001:   // 4                    
        (num == 4'b0101) ? 8'b10010010:   // 5                    
        (num == 4'b0110) ? 8'b10000010:   // 6                    
        (num == 4'b0111) ? 8'b11111000:   // 7                    
        (num == 4'b1000) ? 8'b10000000:   // 8                    
        (num == 4'b1001) ? 8'b10010000:   // 9                    
        (num == 4'b1010) ? 8'b10001000:   // A                    
        (num == 4'b1011) ? 8'b10000011:   // b                    
        (num == 4'b1100) ? 8'b11000110:   // C                    
        (num == 4'b1101) ? 8'b10100001:   // d                    
        (num == 4'b1110) ? 8'b10000110:   // E                    
        (num == 4'b1111) ? 8'b10001110:   // F                    
        8'b11111111   // default i.e. all segments off                       
        };    
        endmodule
module twobit_counter(
    input clk,    
    input rst,    
    output reg [1:0] Q
     );
     always @ (posedge(clk), posedge(rst))   
     // When will Always Block Be Triggered
        begin    
            if (rst == 1'b1)        
               // How Output reacts when Reset Is Asserted
     Q <= 2'b0;
 else
     // How Output reacts when Rising Edge of Clock Arrives?
     Q <= Q + 1'b1;
end 
endmodule

module fourbit_counter(    
    input clk,    
    input run,    
    input rst,    
    output reg [3:0] Dig0,    
    output reg [3:0] Dig1,    
    output reg [3:0] Dig2,    
    output reg [3:0] Dig3
        );
                     
     always @ (posedge(clk), posedge(rst))   
     // When will Always Block Be Triggered
        begin    
            if (rst == 1'b1)
            begin // How Output reacts when Reset Is Asserted
                    Dig0 <= 4'b0;
                    Dig1 <= 4'b0;
                    Dig2 <= 4'b0;
                    Dig3 <= 4'b0;
                   end
                else if (run == 1'b1)       
                begin 
     // How Output reacts when Reset Is Asserted
     if (Dig0 == 4'b1001)
                 begin
                  Dig0 <= 4'b0;
                  if (Dig1 == 4'b1001)
                     begin
                     Dig1 <= 4'b0;
                     if (Dig2 == 4'b1001)
                         begin
                         Dig2 <= 4'b0;
                         if (Dig3 == 4'b1001)
                             begin
                             Dig3 <= 1'b0;
                             end
                         else
                             Dig3 <= Dig3 + 1'b1;
                         end
                     else
                         Dig2 <= Dig2 + 1'b1;
                     end
                  else
                     Dig1 <= Dig1 + 1'b1;
                 end
             else
                 Dig0 <= Dig0 + 1'b1;
             end
     end 
     endmodule
     
            module mux_4_1(    
                input [3:0] I0,    
                input [3:0] I1,    
                input [3:0] I2,    
                input [3:0] I3,    
                input [1:0] Sel,    
                output reg [3:0] Y    
                );    
     always @ (Sel, I0, I1, I2, I3)    
        begin        
            case 
            (Sel)
        2'd0:
        Y = I0;
        2'd1:
        Y = I1;
        2'd2:
        Y = I2;
        2'd3:
        Y = I3;            
        default:
        Y = 2'd0;   
        endcase    
        end    
    endmodule
 
    module clk_divider(
        input clk,    
        input rst,    
        output reg clk_div
            ); 
        wire [1:0] din;
        wire [1:0] clkdiv;
    localparam constantNumber = 50000; 
    reg [31:0] count; 
        always @ (posedge(clk), posedge(rst))
            begin    
            if (rst == 1'b1)
                count <= 32'b0;
                else if (count == constantNumber - 1)
                    count <= 32'b0;
                else
                    count <= count + 1;
            end
                always @ (posedge(clk), posedge(rst))
                begin    
                if (rst == 1'b1)
                    clk_div <= 1'b0;
                   else if (count == constantNumber - 1)
                       clk_div <= ~clk_div;
                   else
                       clk_div <= clk_div;
               end
               endmodule
               
        module Controller(
            input Stop, 
            input Start,
            input Inc,
            output Run,
            input clk,
            input rst
            ); 
        // Define State Codes
                localparam S0 = 2'b00;
                localparam S1 = 2'b01;
                localparam S2 = 2'b10;
                localparam S3 = 2'b11; 
            
        //present state, next 
               reg [1:0] pState, nState; 
        // Combinational Logic: Next State Logic 
                        always @ (pState, Start, Stop, Inc, rst)
                            begin 
                                case (pState)
                                S0: 
                                    begin            
                                        if (rst == 1'b1)                
                                            nState = S0;            
                                        else if ((Start == 1'b0) && (Inc == 1'b1))                
                                            nState = S3;            
                                        else if (Start == 1'b1)                
                                            nState = S1;            
                                        else                
                                            nState = S0;            
                                    end        
                                        S1: 
                                    begin            
                                        if (rst == 1'b1)                
                                            nState = S0;            
                                        else if (Stop == 1'b1)                
                                            nState = S0;            
                                        else                
                                            nState = S1;            
                                    end        
                                        S2: 
                                    begin            
                                        if (rst == 1'b1)                
                                            nState = S0;            
                                        else if (Inc == 1'b0)                
                                            nState = S0;            
                                        else                
                                            nState = S2;            
                                    end        
                                        S3: 
                                    begin            
                                        if (rst == 1'b1)                
                                            nState = S0;            
                                        else                
                                            nState = S2;            
                                    end        
            
                                default:            
                                    nState = S0;    
                            endcase
                    end 
        // State Registers
                always @ (posedge(clk), posedge(rst))
                    begin    
                if (rst == 1'b1)        
                    pState <= S0;
                   else
                       pState <= nState;
               end
                
               // Output Logic
               assign Run = pState[0];
endmodule