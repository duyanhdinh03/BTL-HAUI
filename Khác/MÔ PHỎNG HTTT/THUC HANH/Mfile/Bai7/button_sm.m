function varargout = button_sm(varargin)
% BUTTON_SM MATLAB code for button_sm.fig
%      BUTTON_SM, by itself, creates a new BUTTON_SM or raises the existing
%      singleton*.
%
%      H = BUTTON_SM returns the handle to a new BUTTON_SM or the handle to
%      the existing singleton*.
%
%      BUTTON_SM('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in BUTTON_SM.M with the given input arguments.
%
%      BUTTON_SM('Property','Value',...) creates a new BUTTON_SM or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before button_sm_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to button_sm_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help button_sm

% Last Modified by GUIDE v2.5 29-Oct-2021 17:39:34

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @button_sm_OpeningFcn, ...
                   'gui_OutputFcn',  @button_sm_OutputFcn, ...
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


% --- Executes just before button_sm is made visible.
function button_sm_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to button_sm (see VARARGIN)

% Choose default command line output for button_sm
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes button_sm wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = button_sm_OutputFcn(hObject, eventdata, handles) 
sim 'baitap'
axes(handles.axes1);
plot(time,data);
grid on

varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
sim 'baitap'
axes(handles.axes1);
plot(time,data);
% grid on
