class CreateActivities < ActiveRecord::Migration
  def change
    create_table :activities do |t|
      t.date :date
      t.string :activity_type
      t.float :duration

      t.timestamps
    end
  end
end
