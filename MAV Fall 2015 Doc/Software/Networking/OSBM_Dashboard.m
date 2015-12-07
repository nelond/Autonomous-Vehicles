function varargout = OSBM_Dashboard(varargin)
% OSBM_DASHBOARD MATLAB code for OSBM_Dashboard.fig
%      OSBM_DASHBOARD, by itself, creates a new OSBM_DASHBOARD or raises the existing
%      singleton*.
%
%      H = OSBM_DASHBOARD returns the handle to a new OSBM_DASHBOARD or the handle to
%      the existing singleton*.
%
%      OSBM_DASHBOARD('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in OSBM_DASHBOARD.M with the given input arguments.
%
%      OSBM_DASHBOARD('Property','Value',...) creates a new OSBM_DASHBOARD or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before OSBM_Dashboard_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to OSBM_Dashboard_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help OSBM_Dashboard

% Last Modified by GUIDE v2.5 06-Dec-2015 21:40:37

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @OSBM_Dashboard_OpeningFcn, ...
                   'gui_OutputFcn',  @OSBM_Dashboard_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before OSBM_Dashboard is made visible.
function OSBM_Dashboard_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to OSBM_Dashboard (see VARARGIN)

% Choose default command line output for OSBM_Dashboard
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes OSBM_Dashboard wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = OSBM_Dashboard_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
