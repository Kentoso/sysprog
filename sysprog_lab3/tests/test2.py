# Importing necessary modules with aliases for obfuscation
import os as file_operator
from tensorflow import keras as neural_framework
import pathlib as path_tool
from joblib import load as data_loader
import warnings as caution_manager
import pandas as data_table
import catboost as boosting_tool

# Ensure that default warnings are set
caution_manager.filterwarnings("default")

# Resolve the path based on provided directory or file part
def path_resolver(part):
    # Joining path components to get the full path
    return file_operator.path.join(path_tool.Path(file_operator.path.dirname(file_operator.path.realpath(__file__))), part)

# Setting up path for boosted model and loading it
boosted_model_path = path_resolver('models/optimal_boost')
boosted_model = boosting_tool.CatBoostClassifier().load_model(boosted_model_path)

# Setting up path for neural model and loading it
neural_model_path = path_resolver('models/optimal_neural.h5')
neural_model = neural_framework.models.load_model(neural_model_path)

# Loading data translator from a joblib file
data_translator = data_loader(path_resolver('data_translator.joblib'))

# Function to reshape and process the data
def data_shaper(batch_data) -> data_table.DataFrame:
    # Columns to be considered for subsetting the data
    column_set1 = ['node_A', 'node_B', 'node_C', 'node_D']
    column_set2 = ['node_A', 'node_B', 'node_E']
    
    # Creating subsets based on dropped columns
    sub_data1 = batch_data.drop(columns=column_set1)
    sub_data2 = batch_data.drop(columns=column_set2)
    
    # Transforming data using loaded translator
    refined_data = data_translator.transform(sub_data2)

    # Obtaining outcomes using the trained models
    outcome_A = boosted_model.predict_proba(sub_data1.drop(columns=['is_trial']))
    outcome_B = neural_model.predict(refined_data)

    # Assigning results to new columns in the batch data
    batch_data['out_A'] = outcome_A
    batch_data['out_B'] = (outcome_A > 0.3).astype(int)
    batch_data['out_B'] = batch_data[['is_trial', 'out_B']].apply(lambda x: 1 if x['is_trial'] == 0 else x['out_B'], axis=1)
    batch_data['out_C'] = outcome_B
    batch_data['final_out'] = batch_data[['out_B','out_C']].apply(lambda x: x['out_B'] * x['out_C'], axis=1)

    # Return processed data
    return batch_data[['node_A', 'node_B', 'node_C', 'out_A', 'out_B', 'out_C', 'final_out']]
    