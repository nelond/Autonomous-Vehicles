function msfcn_varpulse(block)
% Level-2 MATLAB file S-function to implement a variable pulse width generator
%   Copyright 1990-2009 The MathWorks, Inc.

% This S-function takes a desired pulse width (in percentage
% of the period of a Pulse Generator block) and sets the PulseWidth
% property in a Pulse Generator block. The result is a variable-width
% pulse signal. The S-function assumes the model contains only one Pulse
% Generator block and modifies the PulseWidth of that block.

setup(block);

%endfunction


function setup(block)

% Register number of ports
block.NumInputPorts  = 2;
block.NumOutputPorts = 0;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';

% Register sample times
block.SampleTimes = [0 0];

% Set the block simStateCompliance to custom
block.SimStateCompliance = 'CustomSimState';

% Register methods
block.RegBlockMethod('PostPropagationSetup', @DoPostPropSetup);
block.RegBlockMethod('InitializeConditions', @InitializeConditions);
block.RegBlockMethod('Start', @Start);
block.RegBlockMethod('Outputs', @Outputs);
block.RegBlockMethod('Update', @Update);
block.RegBlockMethod('Terminate', @Terminate);
block.RegBlockMethod('GetSimState', @GetSimState);
block.RegBlockMethod('SetSimState', @SetSimState);

%endfunction


function DoPostPropSetup(block)

% Initialize the Dwork vector
block.NumDworks = 3;

% Dwork(1) stores the value of the next pulse width
block.Dwork(1).Name            = 'x1';
block.Dwork(1).Dimensions      = 1;
block.Dwork(1).DatatypeID      = 0;      % double
block.Dwork(1).Complexity      = 'Real'; % real
block.Dwork(1).UsedAsDiscState = true;

% Dwork(2) stores the handle of the Pulse Generator block
block.Dwork(2).Name            = 'BlockHandle';
block.Dwork(2).Dimensions      = 1;
block.Dwork(2).DatatypeID      = 0;      % double
block.Dwork(2).Complexity      = 'Real'; % real
block.Dwork(2).UsedAsDiscState = false;

% Dwork(3) stores the value of the next period
block.Dwork(3).Name            = 'x2';
block.Dwork(3).Dimensions      = 1;
block.Dwork(3).DatatypeID      = 0;      % double
block.Dwork(3).Complexity      = 'Real'; % real
block.Dwork(3).UsedAsDiscState = true;

%endfunction


function Start(block)

% Populate the Dwork vector
block.Dwork(1).Data = 0;

% Obtain the Pulse Generator block handle
pulseGen = find_system(gcs,'BlockType','DiscretePulseGenerator');
blockH = get_param(pulseGen{1},'Handle');
block.Dwork(2).Data = blockH;

block.Dwork(3).Data = 1;

%endfunction


function InitializeConditions(block)

% Set the initial pulse width value
set_param(block.Dwork(2).Data, 'PulseWidth', num2str(50));
set_param(block.Dwork(2).Data, 'Period', num2str(0.02));

%endfunction


function Outputs(block)

% Update the pulse width value
set_param(block.Dwork(2).Data, 'PulseWidth', num2str(block.InputPort(1).data));
set_param(block.Dwork(2).Data, 'Period', num2str(block.InputPort(2).data));

%endfunction


function Update(block)

% Store the input value in the Dwork(1)
block.Dwork(1).Data = block.InputPort(1).Data;
block.Dwork(3).Data = block.InputPort(2).Data;

%endfunction


function Terminate(block)

%endfunction
    
function outSS = GetSimState(block)
% The value stored in DWork(1) is this blocks SimState
outSS = [block.Dwork(1).Data block.Dwork(3).Data];
%endfunction

function SetSimState(block, inSS)
% Restore the SimState passed in to DWork(1)
block.Dwork(1).Data = inSS(1);
set_param(block.Dwork(2).Data, 'PulseWidth', num2str(block.Dwork(1).Data));
block.Dwork(3).Data = inSS(2);
set_param(block.Dwork(2).Data, 'Period', num2str(block.Dwork(3).Data));

%endfunction
