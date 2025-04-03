#!/bin/bash

# Submit simulation job
sim_job_id=$(sbatch simulation.job | awk '{print $4}')
echo "Submitted simulation job with ID: $sim_job_id"

# Submit evaluation job with dependency on simulation job
eval_job_id=$(sbatch --dependency=afterok:$sim_job_id evaluation.job | awk '{print $4}')
echo "Submitted evaluation job with ID: $eval_job_id (will run after simulation job $sim_job_id completes)"